/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"
#include "StockRequester.h"
#include "StockView.h"

#include <stdio.h>

MainWindow::MainWindow(BRect rect) 
	:BWindow(rect, "MainWindow", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_QUIT_ON_WINDOW_CLOSE ) {
	
	BRect frame = Bounds();
	fStockView = new StockView(frame);
	AddChild(fStockView);
}

MainWindow::~MainWindow() {
	//delete fStockView;
}
