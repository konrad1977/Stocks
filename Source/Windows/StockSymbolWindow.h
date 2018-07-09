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
class SearchView;
class StockSymbolWindow: public BWindow {
public:
	StockSymbolWindow(BRect rect);	
	~StockSymbolWindow();
	
	virtual void MessageReceived(BMessage *message);							
private:
			void SetItems(BList *listItems);
			BList * Filtered(const char *filter);
			
	StockRequester *fStockRequester;
	SearchView *fSearchView;
	BListView *fSymbolListView;
	BScrollView *fScrollView;
	BList *fStockSymbolListItems;
	BList *fCurrentFilter;
	bool fHasFilter;
};

#endif // _H
