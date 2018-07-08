/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
*/
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SupportDefs.h>
#include <Window.h>

class StockListView;
class StockRequester;
class MainWindow: public BWindow {
public:
	MainWindow(BRect rect);	
	~MainWindow();
	
	virtual void MessageReceived(BMessage *message);
private:
	void _RequestData();	
	StockListView *fStockListView;	
	StockRequester *fStockRequester;	
};

#endif
