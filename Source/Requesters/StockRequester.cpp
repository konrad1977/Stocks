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
	//_RequestCompanyInformation();
}

void 
StockRequester::RequestBatchData() {
		
	NetRequester requester(fHandler, QUOTE);	
			
	const char *requestUrl = fBuilder->CreateBatchPath();
	
	BUrl url = BUrl(requestUrl);	
	BUrlRequest* request = BUrlProtocolRoster::MakeRequest(url, &requester);

	thread_id thread = request->Run();
	wait_for_thread(thread, NULL);
		
	delete request;
}

void 
StockRequester::RemoveStockSymbol(const char *symbol) {
	fBuilder->RemoveSymbol(symbol);
}

void
StockRequester::AddStockSymbol(const char *symbol) {
	fBuilder->AddSymbol(symbol);
}

void
StockRequester::BatchMakeEmpty() {
	fBuilder->MakeEmpty();
}

void 
StockRequester::DownloadSymbols() {
	
	NetRequester requester(fHandler, STOCK_SYMBOLS);
	BUrl url = BUrl("https://api.iextrading.com/1.0/ref-data/symbols");	
	BUrlRequest* request = BUrlProtocolRoster::MakeRequest(url, &requester);

	thread_id thread = request->Run();
	wait_for_thread(thread, NULL);
	delete request;
}

void 
StockRequester::RequestStockInformation(const char *symbol) {
	
	NetRequester requester(fHandler, COMPANY_INFORMATION);	
	fBuilder->SetCompany(symbol);
				
	BUrl url = BUrl(fBuilder->CreateCompanyPath(Company));
	BUrlRequest* request = BUrlProtocolRoster::MakeRequest(url, &requester);

	thread_id thread = request->Run();
	wait_for_thread(thread, NULL);
		
	delete request;
}							
