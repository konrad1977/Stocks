/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef STOCKSYMBOLWINDOW_H
#define STOCKSYMBOLWINDOW_H


#include <SupportDefs.h>
#include <Window.h>


class BScrollView;
class BListView;
class BList;
class SettingsManager;
class StockRequester;
class SearchView;
class StockListExtendedView;
class StockSymbolWindow: public BWindow {
public:
	StockSymbolWindow();	
	~StockSymbolWindow();
	
	virtual	bool QuitRequested();
	virtual void MessageReceived(BMessage *message);	
			void SetTarget(BHandler *handler);		
			void SetStockSymbols(BList *symbols);	
private:
	const char*  SymbolAtIndex(int32 index);
			void InitLayout();
			void SetItems(BList *listItems);
			void ShowAlert(const char *title, const char *message);
			void ApplyFilter(BString filter);
			bool HasSymbolInPortfolio(const char *symbol);
				
			
			void HandleAddToPortfolio(BMessage *message);
			void HandleSelection(BMessage *message);
			void HandleCompanyInformation(BMessage *message);
			void HandleSearch(BMessage *message);
			void HandleQuoteInformation(BMessage *message);
			
			BList 			*Filtered(BString filter);			
			
	SearchView 				*fSearchView;
	StockListExtendedView 	*fStockListExtendedView;
	BListView 				*fSymbolListView;
	BScrollView 			*fScrollView;
	BList 					*fStockSymbolListItems;
	BList 					*fCurrentFilter;
	
	BMessenger 				*fMessenger;
	bool fHasFilter;	
};

#endif // _H
