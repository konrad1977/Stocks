/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Locker.h>
#include <UrlProtocolRoster.h>
#include <UrlRequest.h>

#include "StockRequester.h"
#include "UrlBuilder.h"

#include <SupportDefs.h>
#include <stdio.h>


StockRequester::StockRequester(BHandler *handler) 
	:fHandler(handler)
	,fBuilder(NULL)	{ 	
	fBuilder = new UrlBuilder("https://api.iextrading.com/1.0/stock");
}

StockRequester::~StockRequester() {
	delete fBuilder;
}

void 
StockRequester::Remove(const char *symbol) {
	fBuilder->RemoveSymbol(symbol);
}

void
StockRequester::Add(const char *symbol) {
	fBuilder->AddSymbol(symbol);
}

void
StockRequester::ResetUrlList() {
	fBuilder->MakeEmpty();
}

void 
StockRequester::DownloadSymbols() {

	NetRequester requester(fHandler, STOCK_SYMBOLS);
	RunRequest("https://api.iextrading.com/1.0/ref-data/symbols", &requester);
}

void 
StockRequester::RequestBatchData() {

	if (fBuilder->IsBatchRequestValid() == false) {
		return;
	}
	
	NetRequester requester(fHandler, QUOTE);	
	const char *requestUrl = fBuilder->CreateBatchPath();
	RunRequest(requestUrl, &requester);
}

void 
StockRequester::RequestStockInformation(const char *symbol) {
	
	NetRequester requester(fHandler, COMPANY_INFORMATION);	
	fBuilder->SetCompany(symbol);
	RunRequest(fBuilder->CreateCompanyPath(Company), &requester);
}	

void
StockRequester::RunRequest(const char *urlStr, NetRequester *requester) {

	BUrl url = BUrl(urlStr);	
	BUrlRequest* request = BUrlProtocolRoster::MakeRequest(url, requester);

	thread_id thread = request->Run();
	wait_for_thread(thread, NULL);
	delete request;
}						
