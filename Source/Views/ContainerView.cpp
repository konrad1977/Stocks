/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ContainerView.h"
#include <Dragger.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <List.h>
#include <MessageRunner.h>
#include <Alert.h>

#include "SettingsManager.h"
#include "Portfolio.h"
#include "Quote.h"
#include "QuoteListItem.h"
#include "StockRequester.h"
#include "Constants.h"
#include <stdio.h>

const float kDraggerSize = 7;

extern const char *kAppSignature;

ContainerView::ContainerView()
	:BView("HaikuStocks", B_WILL_DRAW | B_DRAW_ON_CHILDREN)
	,fDragger(NULL)
	,fQuoteListView(NULL)
	,fStockRequester(NULL)
	,fCurrentSymbols(NULL)
	,fMessenger(NULL)
	,fIsReplicant(false)
	,fDownloadThread(-1)
	,fAutoUpdateRunner(NULL)
	,fPortfolio(NULL)
{	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetupViews();
	LoadSavedData();
}
	
ContainerView::ContainerView(BMessage *archive) 
	:BView(archive)
	,fDragger(NULL)
	,fQuoteListView(NULL)
	,fStockRequester(NULL)
	,fMessenger(NULL)
	,fIsReplicant(true)
	,fDownloadThread(-1)
	,fAutoUpdateRunner(NULL)
	,fPortfolio(NULL)
{	
	SetViewColor(B_TRANSPARENT_COLOR);
	SetupViews();
	LoadSavedData();
}

ContainerView::~ContainerView() {
	delete fStockRequester;
	delete fAutoUpdateRunner;
	delete fMessenger;
}

status_t
ContainerView::Archive(BMessage* into, bool deep) const {
	
	status_t status;
	status = into->AddString("add_on", kAppSignature);
	return BView::Archive(into, false);
}

BArchivable*	
ContainerView::Instantiate(BMessage* archive) {
	return new ContainerView(archive);
}

status_t	
ContainerView::SaveState(BMessage* into, bool deep) const {
	status_t status;
	return B_OK;
}

void
ContainerView::SetTarget(BHandler *handler) {
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
ContainerView::SendEmptyListMessage() {
	BList *list = CurrentPortfolio()->CurrentSymbols();	
	if (list == NULL || list->CountItems() == 0) {
		BMessage emptyListMessage(kEmptyListMessage);
		fMessenger->SendMessage(&emptyListMessage);
	}
}

void
ContainerView::LoadSavedData() {
	BList *list = CurrentPortfolio()->CurrentSymbols();	
	for (int32 index = 0; index<list->CountItems(); index++) {
		char *symbol = static_cast<char *>(list->ItemAt(index));
		Requester()->Add(symbol);
	}	
}

Portfolio*
ContainerView::CurrentPortfolio() {
	if (fPortfolio == NULL) {
		fPortfolio = new Portfolio();
	}
	return fPortfolio;
}

StockRequester* 
ContainerView::Requester() {
	if (fStockRequester == NULL) {
		fStockRequester = new StockRequester(this);
	}
	return fStockRequester;
}

void
ContainerView::InitAutoUpdate() {

	BMessenger view(this);
	bigtime_t seconds = 10;
	BMessage autoUpdateMessage(kAutoUpdateMessage);
	fAutoUpdateRunner = new BMessageRunner(view, &autoUpdateMessage, (bigtime_t) seconds * 1000 * 1000);
}

void
ContainerView::AttachedToWindow() {
	
	CurrentPortfolio()->SetTarget(this);
	RequestData();
	SendEmptyListMessage();
	InitAutoUpdate();
	
	BView::AttachedToWindow();
}

void 
ContainerView::ShowAlert(const char *title, const char *message) {
	BAlert *alert = new BAlert(title, message, "Ok");
	alert->SetType(B_INFO_ALERT);
	alert->Go();
}

void
ContainerView::MessageReceived(BMessage *message) {
	
	switch (message->what) {
		case kPortfolioRemovedSymbolMessage:  {
			BString symbol;
			if (message->FindString("symbol", &symbol) == B_OK) {
				Requester()->Remove(symbol.String());
				RequestData();
			}
			break;
		}

		case kPortfolioAddedSymbolMessage: {
			BString symbol;
			if (message->FindString("symbol", &symbol) == B_OK) {
				Requester()->Add(symbol.String());
				RequestData();
			}
			break;
		}
		
		case kUseSmallQuoteSize: {
			UpdateQuoteItemSizes(SMALL);
			break;
		}

		case kUseNormalQuoteSize: {
			UpdateQuoteItemSizes(NORMAL);
			break;
		}

		case kUseLargeQuoteSize: {
			UpdateQuoteItemSizes(LARGE);
			break;
		}
			
		case kAutoUpdateMessage:{
			RequestData();
			break;
		}
		
		case kRemoveSelectedListItem: {
			RemoveSelectedListItem();
			break;
		}
	
		case kUpdateQuoteBatchMessage: {
			HandleQuotes(*message);
			break;
		}
		
		case kPortfolioButtonPressedMessage: {	
			CurrentPortfolio()->HandlePortfolioUpdate(message);
			break;
		}
		
		case B_ABOUT_REQUESTED: {
			ShowAlert("Stocks", 
			"Written by Mikael Konradsson, 2018\n"
			"Icon design by Humdinger\n\n"
			"Thanks for downloading!\n");	
			break;
		}
		
		default:
			BView::MessageReceived(message);
			break;
	}
}

void
ContainerView::UpdateQuoteItemSizes(QuoteSize size) {

	if (fQuoteListView == NULL) {
		return;
	}

	SettingsManager manager;
	manager.SetQuoteSize(size);
	
	QuoteSize testSize = manager.CurrentQuoteSize();
	printf("%d %d\n", size, testSize);
	
	const int32 items = fQuoteListView->CountItems();
	for(int32 i = 0; i<items; i++) {
		QuoteListItem *item = dynamic_cast<QuoteListItem*>(fQuoteListView->ItemAt(i));
		item->SetQuoteItemSize(size);
		item->DrawItem(fQuoteListView, fQuoteListView->Bounds(), true);
	}
	fQuoteListView->Invalidate();
}

void
ContainerView::DownloadData() {
	
	BList *list = CurrentPortfolio()->CurrentSymbols();
	Requester()->ResetUrlList();	
	
	for (int32 index = 0; index<list->CountItems(); index++) {
		char *symbol = static_cast<char *>(list->ItemAt(index));
		Requester()->Add(symbol);
	}
	Requester()->RequestBatchData();
}

int32 
ContainerView::DownloadDataFunc(void *cookie) {
	ContainerView *view = static_cast<ContainerView *>(cookie);
	view->DownloadData();
	return 0;
}

void
ContainerView::RequestData() {
	
	StopActiveRequest();
	
	fDownloadThread = spawn_thread(&DownloadDataFunc, "Download Data", B_NORMAL_PRIORITY, this);
	if (fDownloadThread >= 0)
		resume_thread(fDownloadThread);
}

void
ContainerView::StopActiveRequest() {
	if (fDownloadThread == -1) {
		return;
	}
	wait_for_thread(fDownloadThread, NULL);
	fDownloadThread = -1;
}

void 
ContainerView::RemoveSelectedListItem() {
	if (fQuoteListView == NULL) {
		return;
	}
	
	int32 selectedIndex = fQuoteListView->CurrentSelection();
	if (selectedIndex != -1) {
		QuoteListItem  *listItem = dynamic_cast<QuoteListItem*>(fQuoteListView->ItemAt(selectedIndex));
		if (listItem && listItem->CurrentQuoteItem()) {
			const char *symbol = listItem->CurrentQuoteItem()->symbol.String();
			CurrentPortfolio()->Remove(symbol);
			fQuoteListView->RemoveItem(selectedIndex);
		}
	} else {
		BAlert *alert = new BAlert("No selection", "No item selected in the list", "Ok");
		alert->SetType(B_WARNING_ALERT);
		alert->Go();
	}
}

/*void ContainerView::UpdateItemWithQuote(Quote *quote) {

	int32 itemCount = fQuoteListView->CountItems();
	bool foundItem = false;
	for(int32 i = 0; i<itemCount; i++) {
		QuoteListItem *listItem = dynamic_cast<QuoteListItem*>(fQuoteListView->ItemAt(i));
		if (listItem && listItem->CurrentQuoteItem()->isEqual(*quote)) {
			listItem->SetQuote(quote);
			fQuoteListView->InvalidateItem(i);
			foundItem = true;
			return;
		}
	}
	
	SettingsManager *manager = new SettingsManager();
	QuoteSize size = manager->CurrentQuoteSize();
	delete manager;
	
	if (foundItem == false) {
		fQuoteListView->AddItem(new QuoteListItem(quote, fIsReplicant, size));
	}
}
*/
void
ContainerView::HandleQuotes(BMessage message) {
		
	if (fQuoteListView == NULL) {
		return;
	}

	SettingsManager manager;
	QuoteSize size = manager.CurrentQuoteSize();
	printf("ContainerView::%s QuoteSize(%d)\n", __FUNCTION__, size);

	fQuoteListView->MakeEmpty();	
	
	BMessage symbolMessage;			
	if (message.FindMessage("Quotes", &symbolMessage) == B_OK) {			
		char *name;
		uint32 type;
		int32 count;
				
		for (int32 i = 0; symbolMessage.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
			
			BMessage currentMessage;
			if (symbolMessage.FindMessage(name, &currentMessage) != B_OK) {
				continue;
			}
				
			BMessage quoteMsg;
			if (currentMessage.FindMessage("quote", &quoteMsg) != B_OK) {
				continue;
			}			
			
			Quote *quote = new Quote(quoteMsg);
			fQuoteListView->AddItem(new QuoteListItem(quote, fIsReplicant, size));
			//UpdateItemWithQuote(quote);
		}
	}
}

void
ContainerView::SetupViews() {
	
	fQuoteListView = new BListView("Stocks", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	fQuoteListView->SetViewColor( B_TRANSPARENT_COLOR );

	BSize draggerSize = BSize(kDraggerSize,kDraggerSize);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fQuoteListView)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.SetExplicitMinSize(draggerSize)
			.SetExplicitMaxSize(draggerSize)
			.Add(fDragger = new BDragger(this))
		.End()
	.End();
}
