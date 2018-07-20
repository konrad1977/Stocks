/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 
#ifndef STOCKREQUESTER_H
#define STOCKREQUESTER_H

#include <SupportDefs.h>
#include <Handler.h>
#include "NetRequester.h"

class UrlBuilder;
class StockRequester {
public:
	StockRequester(BHandler *handler);
	~StockRequester();
	
	void RequestBatchData();
	void RequestData();
	void DownloadSymbols();
	void AddStockSymbol(const char *symbol);
	void RequestStockInformation(const char *symbol);							
private:
	
	void _RequestCompanyInformation();
	void _RequestNews();
	
	BHandler *fHandler;
	
	UrlBuilder *fBuilder;
	BList fSymbolList;
};

#endif // _H
