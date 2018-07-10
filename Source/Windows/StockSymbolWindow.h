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
class StockListExtendedView;
class StockSymbolWindow: public BWindow {
public:
	StockSymbolWindow(BRect rect);	
	~StockSymbolWindow();
	
	virtual void MessageReceived(BMessage *message);							
private:
			void InitLayout();
			void SetItems(BList *listItems);
			void ApplyFilter(BString filter);
			
			void HandleSelection(BMessage *message);
			void HandleCompanyInformation(BMessage *message);
			void HandleUpdate(BMessage *message);
			void HandleSearch(BMessage *message);
			
			BList * Filtered(BString filter);
			
			
	StockRequester *fStockRequester;
	SearchView *fSearchView;
	StockListExtendedView *fStockListExtendedView;
	BListView *fSymbolListView;
	BScrollView *fScrollView;
	BList *fStockSymbolListItems;
	BList *fCurrentFilter;
	bool fHasFilter;
};

#endif // _H
