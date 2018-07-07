/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockView.h"
#include "StockRequester.h"
#include "MessageConstants.h"

#include <stdio.h>
#include "Company.h"

StockView::StockView(BRect rect) 
	:BView(rect, "StockView", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS) {
	rgb_color color = { 255, 0, 0 };
	SetViewColor(color);
	
	fStockRequester = new StockRequester(this);
	fStockRequester->AddStockSymbol("aapl");
	fStockRequester->AddStockSymbol("googl");
} 

StockView::~StockView() {
	delete fStockRequester;
}

void 
StockView::AttachedToWindow() {
	fStockRequester->RequestData();
}

void
StockView::MessageReceived(BMessage *message) {

	switch (message->what) {
		case kUpdateCompanyMessage: {
			
			BMessage companyMessage;
			message->FindMessage("Company", &companyMessage);
			Company *company = new Company(companyMessage);
			company->PrintToStream();
			delete company;
			break;
		}
		default:
			break;
	}
//	message->PrintToStream();
}
