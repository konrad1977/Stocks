/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
*/
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SupportDefs.h>
#include <Window.h>
#include "QuoteListItem.h"

class StockRequester;
class SettingsWindow;
class StockSymbolWindow;
class BMenuBar;
class ContainerView;
class Portfolio;
class MainWindow: public BWindow {
public:
	MainWindow(Portfolio *portfolio);	
	~MainWindow();
	
	virtual void MessageReceived(BMessage *message);
	
private:
			void SetSelectedMenuFromQuoteSize(QuoteSize size);
			void InitQuoteSize();
			void SetupViews();	
			void DownloadStockSymbols();
			void HandleStockSearchSymbols(BMessage *message);
			void ShowStockWindow();
			void SendToContainerView(BMessage *message);
				
	StockSymbolWindow 	*SymbolWindow();
	SettingsWindow		*CurrentSettingWindow();
	
	BMenuBar 			*fMenuBar;		
	ContainerView 		*fContainerView;
	
	StockRequester 		*fStockRequester;
	StockSymbolWindow 	*fStockSymbolWindow;
	SettingsWindow		*fSettingsWindow;
	
	BList 				*fSymbolList;
	bool 				fShowStockSymbolListWhenDone;
	bool				fStockSymbolsLoaded;
	
	BMenuItem 			*fRemoveSelected;
	BMenuItem			*fMinimalItem;
	BMenuItem			*fNormalItem;
	BMenuItem			*fExtenededItem;
	Portfolio			*fPortfolio;
};

#endif
