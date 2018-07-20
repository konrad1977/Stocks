/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"
#include "StockRequester.h"
#include "Company.h"
#include "Quote.h"
#include "MessageConstants.h"
#include "QuoteListItem.h"

#include <Application.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Locale.h>

#include <StringItem.h>
#include <LayoutBuilder.h>
#include <stdio.h>
#include <ListView.h>
#include <StringItem.h>

#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

MainWindow::MainWindow(BRect rect) 
	:BWindow(rect, "Portfolio", B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE )
	,fMenuBar(NULL)
	,fStockRequester(NULL) 
	,fStockListView(NULL) {
	
	SetupViews();
}

MainWindow::~MainWindow() {
	delete fStockRequester;
}

StockRequester *
MainWindow::Requester() {
	if (fStockRequester == NULL) {
		fStockRequester = new StockRequester(this);
	}
	return fStockRequester;
}

void 
MainWindow::RequestData() {
	Requester()->RequestBatchData();
}

void 
MainWindow::AddSymbol(const char *symbol) {
	Requester()->AddStockSymbol(symbol);
}

void
MainWindow::SetupViews() {

	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	SetLayout(layout);
	
	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu("Main")
			.AddItem("About...", B_ABOUT_REQUESTED, 'A')
			.AddSeparator()
			.AddItem("Quit", B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu("Settings")
			.AddItem("Add symbol...", kShowSearchWindowMessage, 'S')
		.End();
	

	fStockListView = new BListView("Stocks", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(fMenuBar)
		.Add(fStockListView);	
}

void
MainWindow::HandleQuotes(BMessage message) {

	fStockListView->MakeEmpty();
	
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
			fStockListView->AddItem(new QuoteListItem(quote));
		}
	}
}

void
MainWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
			
		case kUpdateQuoteBatchMessage: {
			HandleQuotes(*message);
			break;
		}
		
		case kShowSearchWindowMessage: {
			BMessage *showSearchWindowMessage = new BMessage(kShowSearchWindowMessage);
			be_app_messenger.SendMessage(showSearchWindowMessage);
			delete showSearchWindowMessage;
			break;
		}
		
		case B_ABOUT_REQUESTED:
			break;
			
		case kUpdateCompanyMessage: {			
			BMessage companyMessage;
			message->FindMessage("Company", &companyMessage);
			Company *company = new Company(companyMessage);
			fStockListView->AddItem(new BStringItem(company->name));
			break;
		}
		default:
			break;
	}
}

