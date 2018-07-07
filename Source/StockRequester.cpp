/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Locker.h>
#include <UrlProtocolRoster.h>
#include <UrlRequest.h>

#include "StockRequester.h"
#include "NetRequester.h"
#include "UrlBuilder.h"

#include <SupportDefs.h>
#include <stdio.h>


StockRequester::StockRequester(BHandler *handler) 
	:fHandler(handler) { 
	
	fSymbolList = BList();	
	
	fBuilder = new UrlBuilder("https://api.iextrading.com/1.0/stock");
}

StockRequester::~StockRequester() {
	delete fBuilder;
}

void
StockRequester::RequestData() {
	_RequestCompanyInformation();
}

void
StockRequester::AddStockSymbol(const char *symbol) {
	if (fSymbolList.HasItem(symbol)) {
		return;
	}
	fSymbolList.AddItem((void*)symbol);
}

void
StockRequester::_RequestCompanyInformation() {
	
	const int items = fSymbolList.CountItems();
	
	for (int i = 0; i<items; i++) {
		
		NetRequester requester(fHandler);
		
		const char *symbol = (const char *)fSymbolList.ItemAt(i);
		
		if (symbol == NULL) 
			continue;
			
		fBuilder->SetCompany(symbol);
		
		printf("Symbol: %s\n", fBuilder->CreatePath(Company));
		
		BUrl url = BUrl(fBuilder->CreatePath(Company));
		
		BUrlRequest* request = BUrlProtocolRoster::MakeRequest(url, &requester);

		thread_id thread = request->Run();
		wait_for_thread(thread, NULL);
		
		delete request;
	}
}
