/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

const char *kSignature = "application/x-vnd.konradsson.HaikuStocks";

#include "App.h"
#include "MainWindow.h"
#include "StockSymbolWindow.h"
#include "MessageConstants.h"

#include <stdio.h>

App::App(void)
	:BApplication(kSignature)
	,fWindow(NULL)
	,fStockSymbolWindow(NULL){ 
	
	fWindow = new MainWindow(BRect(150,150,640,480));
	fWindow->Show();	
}

App::~App() {
//	delete fWindow;
//	delete fStockSymbolWindow;
}

StockSymbolWindow *
App::SymbolWindow() {
	if (fStockSymbolWindow == NULL) {			
		fStockSymbolWindow = new StockSymbolWindow();
	}
	return fStockSymbolWindow;
}

void
App::MessageReceived(BMessage *message) {
	switch (message->what) {
		
		case kShowSearchWindowMessage:
			SymbolWindow()->Show();
		break;
		
		default:
			message->PrintToStream();
		break;
	}
}

int main() {
	App app;
	app.Run();
	return 0; 	
}
