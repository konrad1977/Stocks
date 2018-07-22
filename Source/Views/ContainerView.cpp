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

#include "Quote.h"
#include "QuoteListItem.h"
#include "SettingsManager.h"
#include "StockRequester.h"
#include "MessageConstants.h"
#include <stdio.h>

const float kDraggerSize = 7;

extern const char *kAppSignature;

ContainerView::ContainerView()
	:BView("ContainerView", B_WILL_DRAW | B_FRAME_EVENTS)
	,fDragger(NULL)
	,fQuoteListView(NULL)
	,fSettingsManager(NULL) 
	,fStockRequester(NULL)
	,fCurrentSymbols(NULL)
{	
	SetupViews();
}
	
ContainerView::ContainerView(BMessage *archive) 
	:BView(archive)
	,fDragger(NULL)
	,fQuoteListView(NULL)
	,fSettingsManager(NULL) 
	,fStockRequester(NULL)
	,fCurrentSymbols(NULL)
{	
	SetupViews();
}

ContainerView::~ContainerView() {
	delete fStockRequester;
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

StockRequester* 
ContainerView::Requester() {
	if (fStockRequester == NULL) {
		fStockRequester = new StockRequester(this);
	}
	return fStockRequester;
}

void
ContainerView::AttachedToWindow() {
	LoadSymbols();
}

void
ContainerView::MessageReceived(BMessage *message) {
	
	message->PrintToStream();
	
	switch (message->what) {
			
		case kUpdateQuoteBatchMessage: {
			HandleQuotes(*message);
			break;
		}
		default:
			break;
	}
}

void
ContainerView::LoadSymbols() {

	SettingsManager *manager = new SettingsManager();
	
	if (fCurrentSymbols) {
		fCurrentSymbols->MakeEmpty();
	}
	
	fCurrentSymbols = manager->LoadSymbols();
			
	for (int32 index = 0; index<fCurrentSymbols->CountItems(); index++) {
		char *symbol = (char *)fCurrentSymbols->ItemAt(index);
		Requester()->AddStockSymbol(symbol);
		printf("%s\n", symbol);
	}
	
	Requester()->RequestBatchData();
	delete manager;
}

void
ContainerView::HandleQuotes(BMessage message) {

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
			fQuoteListView->AddItem(new QuoteListItem(quote));
		}
	}
}

void
ContainerView::SetupViews() {
	
	fQuoteListView = new BListView("Stocks", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fQuoteListView)
		.AddGroup(B_HORIZONTAL, 0)
				.AddGlue()
				.Add(fDragger = new BDragger(this))
		.End()
	.End();
	
	fDragger->SetExplicitMinSize(BSize(kDraggerSize, kDraggerSize));
	fDragger->SetExplicitMaxSize(BSize(kDraggerSize, kDraggerSize));
}
