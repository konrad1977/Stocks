/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

const char *kSignature = "application/x-vnd.konradsson.HaikuStocks";

#include "App.h"
#include "MainWindow.h"
#include "StockSymbolWindow.h"

App::App(void): 
	BApplication(kSignature) {
	
	fStockSymbolWindow = new StockSymbolWindow(BRect(200,200,900,720));	
	fStockSymbolWindow->Show();
	
	//fWindow = new MainWindow(BRect(150,150,640,480));
	//fWindow->Show();
}

int main() {
	App *app = new App();
	app->Run();
	delete app;
	return 0; 	
}
