/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PORTFOLIOMANAGERWINDOW_H
#define PORTFOLIOMANAGERWINDOW_H


#include <SupportDefs.h>
#include <interface/Window.h>

class BWindow;
class BListView;
class BMenuBar;
class StockSymbolWindow;
class StockRequester;
class Portfolio;
class PortfolioWindow;
class PortfolioManager;
class PortfolioManagerWindow : public BWindow {
public:
	PortfolioManagerWindow();
	~PortfolioManagerWindow();
	
	virtual void MessageReceived(BMessage *message);
	
private:
	StockSymbolWindow* SymbolWindow();
	
			void DownloadStockSymbols();
			void ShowStockWindow();
			void InitLayout();
			void ReloadPortfolios();
			
			BWindow* PortfolioWindowWithName(BString str);
			void ShowWindowWithPortfolio(Portfolio *portfolio);
			void HandleAlreadyExist(BString previousName);
			
			void HandleNewPortfolioMessage(BMessage &message);
			void HandleStockSearchSymbols(BMessage *message);

	StockRequester		*fStockRequester;
	StockSymbolWindow 	*fStockSymbolWindow;	
	PortfolioWindow 	*fPortfolioWindow;
	PortfolioManager 	*fPortfolioManager;
	BMenuBar 			*fMenuBar;
	BMenuItem			*fRemoveSelectedItem;
	BListView 			*fListView;
	
	BList 				*fSymbolList;
	bool 				fShowStockSymbolListWhenDone;
	bool				fStockSymbolsLoaded;
	int32 				fCurrentSelectedItemIndex;
};

#endif // _H
