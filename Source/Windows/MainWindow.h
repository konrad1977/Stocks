/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
*/
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SupportDefs.h>
#include <Window.h>

class StockRequester;
class StockSymbolWindow;
class BMenuBar;
class ContainerView;
class MainWindow: public BWindow {
public:
	MainWindow(BRect rect);	
	~MainWindow();
	
	virtual void MessageReceived(BMessage *message);
	
private:
			void SetupViews();	
			void DownloadStockSymbols();
			void HandleStockSearchSymbols(BMessage *message);
			void ShowStockWindow();
			void SendToContainerView(BMessage *message);
				
	StockSymbolWindow 	*SymbolWindow();
	
	BMenuBar 			*fMenuBar;		
	ContainerView 		*fContainerView;
	
	StockRequester 		*fStockRequester;
	StockSymbolWindow 	*fStockSymbolWindow;
	BList 				*fSymbolList;
};

#endif
