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
NetRequester::_HandleData(BString data) {
	
	BMessenger messenger(fHandler);
	
	BMessage parsedData;
	BJson parser;

	status_t status = parser.Parse(data, parsedData);
	if (status == B_BAD_DATA) {
		printf("JSON Parser error for data:\n%s\n", data.String());
		//BMessage* message = new BMessage(kFailureMessage);
		//messenger.SendMessage(message);
		return;
	}
	
	switch (fType) {
		case COMPANY_INFORMATION: {
			BMessage *objectUpdatedMessage = new BMessage(kUpdateCompanyMessage);
			objectUpdatedMessage->AddMessage("Company", &parsedData);
			messenger.SendMessage(objectUpdatedMessage);
			delete objectUpdatedMessage;
		}
		break;
		
		case STOCK_SYMBOLS: { 
			BMessage *objectUpdatedMessage = new BMessage(kUpdateSymbolMessage);
			objectUpdatedMessage->AddMessage("Symbols", &parsedData);
			messenger.SendMessage(objectUpdatedMessage);
			delete objectUpdatedMessage;
		}
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
