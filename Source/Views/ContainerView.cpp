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

ContainerView::ContainerView(Portfolio *portfolio)
	:BView("HaikuStocks", B_WILL_DRAW | B_DRAW_ON_CHILDREN)
	,fDragger(NULL)
	,fQuoteListView(NULL)
	,fCurrentSymbols(NULL)
	,fMessenger(NULL)
	,fAutoUpdateRunner(NULL)
	,fStockRequester(NULL)
	,fPortfolio(portfolio)
	,fSettingsManager(NULL)
	,fDownloadThread(-1)
	,fIsReplicant(false)
{	
	fSettingsManager = new SettingsManager();
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetupViews();
	LoadSavedData();
}
	
ContainerView::ContainerView(BMessage *archive) 
	:BView(archive)
	,fDragger(NULL)
	,fQuoteListView(NULL)
	,fCurrentSymbols(NULL)
	,fMessenger(NULL)
	,fAutoUpdateRunner(NULL)
	,fStockRequester(NULL)
	,fPortfolio(NULL)
	,fSettingsManager(NULL)
	,fDownloadThread(-1)
	,fIsReplicant(true)
{		

	SetViewColor(B_TRANSPARENT_COLOR);
	SetupViews();	
	fSettingsManager = new SettingsManager();

	BString portfolioName;
	if (archive->FindString("PortfolioName", &portfolioName) != B_OK) {
		return;
	}
	
	fPortfolio = new Portfolio(portfolioName);
	LoadSavedData();
}

ContainerView::~ContainerView() 
{	
	delete fStockRequester;
	delete fAutoUpdateRunner;
	delete fMessenger;
	delete fSettingsManager;
}

status_t
ContainerView::Archive(BMessage* into, bool deep) const 
{
	into->AddString("add_on", kAppSignature);

	if (fPortfolio != NULL) {
		into->AddString("PortfolioName", fPortfolio->Name());
	}
	
	return BView::Archive(into, false);
}

BArchivable*	
ContainerView::Instantiate(BMessage* archive) 
{
	return new ContainerView(archive);
}

status_t	
ContainerView::SaveState(BMessage* into, bool deep) const 
{
	return B_OK;
}

void
ContainerView::SetTarget(BHandler *handler) 
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
ContainerView::SendEmptyListMessage() 
{
	if (fPortfolio == NULL) {
		return;
	}
	
	BList *list = fPortfolio->CurrentSymbols();	
	if (list == NULL || list->CountItems() == 0) {
		BMessage emptyListMessage(kEmptyListMessage);
		fMessenger->SendMessage(&emptyListMessage);
	}
}

void
ContainerView::LoadSavedData() 
{
	if (fPortfolio == NULL) {
		return;
	}
	
	BList *list = fPortfolio->CurrentSymbols();	
	for (int32 index = 0; index<list->CountItems(); index++) {
		char *symbol = static_cast<char *>(list->ItemAt(index));
		Requester()->Add(symbol);
	}	
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
	
	if (fPortfolio == NULL) {
		return;
	}
	
	delete fAutoUpdateRunner;
	
	BMessenger view(this);
	bigtime_t seconds = static_cast<bigtime_t>(fPortfolio->RefreshRate());
	
	BMessage autoUpdateMessage(kAutoUpdateMessage);
	fAutoUpdateRunner = new BMessageRunner(view, &autoUpdateMessage, (bigtime_t) seconds * 1000 * 1000);
}

void
ContainerView::AttachedToWindow() 
{	
	if(fPortfolio) {
		fPortfolio->SetTarget(this);
	}
	
	RequestData();
	SendEmptyListMessage();
	InitAutoUpdate();

	fSettingsManager->StartMonitoring(this);	
	
	BView::AttachedToWindow();
}

void 
ContainerView::ShowAlert(const char *title, const char *message) 
{
	BAlert *alert = new BAlert(title, message, "Ok");
	alert->SetType(B_INFO_ALERT);
	alert->Go();
}

void
ContainerView::MessageReceived(BMessage *message) 
{	
	switch (message->what) {
		
		case kPortfolioUpdatedSettingsMessage: {
			BMessage message(kPortfolioUpdatedSettingsMessage);
			fMessenger->SendMessage(&message);
			break;
		}
		
		case B_NODE_MONITOR: {
			fPortfolio->ReloadSavedData();
			InitAutoUpdate();
			RequestData();
			break;
		}
		
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
			fPortfolio->HandlePortfolioUpdate(message);
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
ContainerView::UpdateQuoteItemSizes(QuoteSize size) 
{
	if (fQuoteListView == NULL || fPortfolio == NULL ) {
		return;
	}

	fPortfolio->SetQuoteSize(size);
	
	const int32 items = fQuoteListView->CountItems();
	for(int32 i = 0; i<items; i++) {
		QuoteListItem *item = dynamic_cast<QuoteListItem*>(fQuoteListView->ItemAt(i));
		item->SetQuoteItemSize(size);
		item->DrawItem(fQuoteListView, fQuoteListView->Bounds(), true);
	}
	fQuoteListView->Invalidate();
}

void
ContainerView::DownloadData() 
{	
	if (fPortfolio == NULL) {
		return;
	}
	
	BList *list = fPortfolio->CurrentSymbols();
	Requester()->ResetUrlList();	
	
	for (int32 index = 0; index<list->CountItems(); index++) {
		char *symbol = static_cast<char *>(list->ItemAt(index));
		Requester()->Add(symbol);
	}
	Requester()->RequestBatchData();
}

int32 
ContainerView::DownloadDataFunc(void *cookie) 
{
	ContainerView *view = static_cast<ContainerView *>(cookie);
	view->DownloadData();
	return 0;
}

void
ContainerView::RequestData() 
{	
	StopActiveRequest();
	
	fDownloadThread = spawn_thread(&DownloadDataFunc, "Download Data", B_NORMAL_PRIORITY, this);
	if (fDownloadThread >= 0)
		resume_thread(fDownloadThread);
}

void
ContainerView::StopActiveRequest() 
{
	if (fDownloadThread == -1) {
		return;
	}
	wait_for_thread(fDownloadThread, NULL);
	fDownloadThread = -1;
}

void 
ContainerView::RemoveSelectedListItem() 
{
	if (fQuoteListView == NULL) {
		return;
	}
	
	int32 selectedIndex = fQuoteListView->CurrentSelection();
	if (selectedIndex != -1) {
		QuoteListItem  *listItem = dynamic_cast<QuoteListItem*>(fQuoteListView->ItemAt(selectedIndex));
		if (listItem && listItem->CurrentQuoteItem()) {
			const char *symbol = listItem->CurrentQuoteItem()->symbol.String();
			fPortfolio->Remove(symbol);
			fQuoteListView->RemoveItem(selectedIndex);
		}
	} else {
		BAlert *alert = new BAlert("No selection", "No item selected in the list", "Ok");
		alert->SetType(B_WARNING_ALERT);
		alert->Go();
	}
}

void
ContainerView::HandleQuotes(BMessage message) 
{		
	if (fQuoteListView == NULL || fPortfolio == NULL) {
		return;
	}
	
	QuoteSize size = fPortfolio->CurrentQuoteSize();
	const uint8 transparency = fPortfolio->Transparency();
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
			QuoteListItem *listItem = new QuoteListItem(quote, fIsReplicant, size);
			listItem->SetTransparency(transparency);
			fQuoteListView->AddItem(listItem);
		}
	}
}

void
ContainerView::SetupViews() 
{	
	fQuoteListView = new BListView("Stocks", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	
	if (fIsReplicant)
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
