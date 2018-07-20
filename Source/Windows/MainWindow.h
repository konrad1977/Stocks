/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
*/
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SupportDefs.h>
#include <Window.h>

class BListView;
class BMenuBar;
class StockRequester;
class MainWindow: public BWindow {
public:
	MainWindow(BRect rect);	
	~MainWindow();
	
	virtual void MessageReceived(BMessage *message);
			void AddSymbol(const char *symbol);
			void RequestData();
private:
			void SetupViews();
			void HandleQuotes(BMessage message); 

	StockRequester *Requester();	
	
	BMenuBar *fMenuBar;		
	StockRequester *fStockRequester;
	BListView *fStockListView;
};

#endif
