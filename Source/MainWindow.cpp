/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"
#include "StockRequester.h"
#include "StockListView.h"
#include "Company.h"
#include "MessageConstants.h"
#include "CompanyListItem.h"

#include <StringItem.h>
#include <stdio.h>

MainWindow::MainWindow(BRect rect) 
	:BWindow(rect, "MainWindow", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_QUIT_ON_WINDOW_CLOSE ) {
	
	BRect frame = Bounds();
	fStockRequester = new StockRequester(this);

	fStockListView = new StockListView(frame);
	AddChild(fStockListView);
	
	fStockRequester->AddStockSymbol("aapl");
	fStockRequester->AddStockSymbol("googl");	
	fStockRequester->RequestData();
}

MainWindow::~MainWindow() {
	delete fStockRequester;
}

void
MainWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
		case kUpdateCompanyMessage: {
			
			BMessage companyMessage;
			message->FindMessage("Company", &companyMessage);
			Company *company = new Company(companyMessage);
			
			fStockListView->AddItem(new CompanyListItem(company));
			
			//company->PrintToStream();
			//delete company;
			break;
		}
		default:
			break;
	}
}

