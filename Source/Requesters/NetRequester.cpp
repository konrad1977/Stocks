/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "NetRequester.h"
#include "Company.h"
#include "MessageConstants.h"
#include <Application.h>

#include <Messenger.h>
#include <Looper.h>
#include <stdio.h>
#include <Json.h>



NetRequester::NetRequester(BHandler *handler, StockRequestType type)
	: BUrlProtocolListener()
	,fType(type) 	
	,fHandler(handler) {
		
}

NetRequester::~NetRequester() {

}

void 
NetRequester::ResponseStarted(BUrlRequest* caller) {
	//printf("Response started\n");
}

void
NetRequester::DataReceived(BUrlRequest* caller, const char* data, off_t position, ssize_t size) {
	fResponseData.Write(data, size);
	//printf("Data recieved (size: %d)\n", size);
}

void
NetRequester::HandleQuoteBatch(BMessage message) {
	BMessenger messenger(fHandler);
	BMessage *objectUpdatedMessage = new BMessage(kUpdateQuoteBatchMessage);
	objectUpdatedMessage->AddMessage("Quotes", &message);
	messenger.SendMessage(objectUpdatedMessage);
	delete objectUpdatedMessage;
}

void 
NetRequester::HandleQuote(BMessage message) {
	
	BMessage quote;
	
	if (message.FindMessage("quote", &quote) == B_OK) {
		BMessenger messenger(fHandler);
		BMessage *objectUpdatedMessage = new BMessage(kUpdateQuoteMessage);
		objectUpdatedMessage->AddMessage("Quote", &quote);
		messenger.SendMessage(objectUpdatedMessage);
		delete objectUpdatedMessage;
	}	
}

void 
NetRequester::HandleStockSymbols(BMessage message) {
	BMessenger messenger(fHandler);
	BMessage *objectUpdatedMessage = new BMessage(kUpdateSymbolMessage);
	objectUpdatedMessage->AddMessage("Symbols", &message);
	messenger.SendMessage(objectUpdatedMessage);
	delete objectUpdatedMessage;
}

void 
NetRequester::HandleCompanyInformation(BMessage message) {
	BMessage company;
	if (message.FindMessage("company", &company) == B_OK) {
		BMessenger messenger(fHandler);
		BMessage *objectUpdatedMessage = new BMessage(kUpdateCompanyMessage);
		objectUpdatedMessage->AddMessage("Company", &company);
		messenger.SendMessage(objectUpdatedMessage);
		delete objectUpdatedMessage;
	}
}

void 
NetRequester::_HandleData(BString data) {
		
	BMessage parsedData;
	BJson parser;

	status_t status = parser.Parse(data, parsedData);
	
	if (status == B_BAD_DATA) {
		BMessenger messenger(fHandler);
		BMessage* message = new BMessage(kUpdateFailed);
		message->AddString("message", data.String());
		messenger.SendMessage(message);
		return;
	}
	
	switch (fType) {

		case QUOTE: {
			HandleQuoteBatch(parsedData);
			break;
		}
	
		case COMPANY_INFORMATION: {
			HandleCompanyInformation(parsedData);
			HandleQuote(parsedData);
			break;
		}

		case STOCK_SYMBOLS:
			HandleStockSymbols(parsedData);
			break;
	
		default:
			break;
	}	
}

void
NetRequester::RequestCompleted(BUrlRequest* caller, bool success) {
	
	BString jsonString;
	jsonString.SetTo(static_cast<const char*>(fResponseData.Buffer()), fResponseData.BufferLength());
	
	_HandleData(jsonString);
}
