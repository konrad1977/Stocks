/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef STOCKSYMBOLWINDOW_H
#define STOCKSYMBOLWINDOW_H


#include <SupportDefs.h>
#include <Window.h>
#include <ListView.h>
#include <ScrollView.h>

class StockRequester;
class StockSymbolWindow: public BWindow {
public:
	StockSymbolWindow(BRect rect);	
	~StockSymbolWindow();
	
	virtual void MessageReceived(BMessage *message);							
private:
	StockRequester *fStockRequester;
	BListView *fSymbolListView;
	BScrollView *fScrollView;
	
};

#endif // _H
