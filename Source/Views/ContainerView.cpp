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

#include "Portfolio.h"
#include "Quote.h"
#include "QuoteListItem.h"
#include "StockRequester.h"
#include "MessageConstants.h"
#include <stdio.h>

const float kDraggerSize = 7;

extern const char *kAppSignature;

ContainerView::ContainerView()
	:BView("HaikuStocks", B_WILL_DRAW | B_DRAW_ON_CHILDREN)
	,fDragger(NULL)
	,fQuoteListView(NULL)
	,fStockRequester(NULL)
	,fCurrentSymbols(NULL)
	,fIsReplicant(false)
	,fDownloadThread(-1)
	,fAutoUpdateRunner(NULL)
	,fPortfolio(NULL)
{	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetupViews();
}
	
ContainerView::ContainerView(BMessage *archive) 
	:BView(archive)
	,fDragger(NULL)
	,fQuoteListView(NULL)
	,fStockRequester(NULL)
	,fIsReplicant(true)
	,fDownloadThread(-1)
	,fAutoUpdateRunner(NULL)
	,fPortfolio(NULL)
{	
	SetViewColor(B_TRANSPARENT_COLOR);
	SetupViews();
}

ContainerView::~ContainerView() {
	delete fStockRequester;
	delete fAutoUpdateRunner;
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
ContainerView::AttachedToWindow() {
	
	CurrentPortfolio()->SetTarget(this);
	
	RequestData();
	
	BMessenger view(this);
	bigtime_t seconds = 10;
	BMessage autoUpdateMessage(kAutoUpdateMessage);
	fAutoUpdateRunner = new BMessageRunner(view, &autoUpdateMessage, (bigtime_t) seconds * 1000 * 1000);
}

void
ContainerView::MessageReceived(BMessage *message) {
	
	switch (message->what) {
		case kPortfolioChangedMessage:  {
			RequestData();
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
		
		default:
			message->PrintToStream();
			break;
	}
}

void
ContainerView::DownloadData() {
	
	BList *list = CurrentPortfolio()->CurrentSymbols();
	Requester()->BatchMakeEmpty();	
	
	for (int32 index = 0; index<list->CountItems(); index++) {
		char *symbol = (char *)list->ItemAt(index);
		Requester()->AddStockSymbol(symbol);
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
		QuoteListItem  *listItem = (QuoteListItem*)fQuoteListView->ItemAt(selectedIndex);
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

void
ContainerView::HandleQuotes(BMessage message) {

	if (fQuoteListView)
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
			fQuoteListView->AddItem(new QuoteListItem(quote, fIsReplicant));
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
