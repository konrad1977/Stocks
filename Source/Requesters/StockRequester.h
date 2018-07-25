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
	void DownloadSymbols();
	
	void ResetUrlList();
	void Add(const char *symbol);
	void Remove(const char *symbol);
	
	void RequestStockInformation(const char *symbol);							
private:
	void RunRequest(const char *url, NetRequester *requester);
	void _RequestNews();
	BHandler *fHandler;	
	UrlBuilder *fBuilder;
};

#endif // _H
