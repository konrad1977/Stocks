/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
*/
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SupportDefs.h>
#include <Window.h>

class StockView;
class MainWindow: public BWindow {
public:
	MainWindow(BRect rect);	
	~MainWindow();
private:
	void _RequestData();	
	StockView *fStockView;		
};

#endif
