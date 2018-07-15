/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

const char *kSignature = "application/x-vnd.konradsson.HaikuStocks";

#include "App.h"
#include "MainWindow.h"
#include "StockSymbolWindow.h"
#include "MessageConstants.h"
#include "SymbolListItem.h"
#include "StockRequester.h"

#include <stdio.h>

App::App(void)
	:BApplication(kSignature)
	,fSymbolList(NULL)
	,fStockRequester(NULL)
	,fWindow(NULL)
	,fStockSymbolWindow(NULL){ 
	
	fStockRequester = new StockRequester(this);
	fStockRequester->DownloadSymbols();
	
	fWindow = new MainWindow(BRect(150,150,640,480));
	fWindow->Show();	
}

App::~App() {

}

StockSymbolWindow *
App::SymbolWindow() {
	if (fStockSymbolWindow == NULL) {		
		fStockSymbolWindow = new StockSymbolWindow();
		fStockSymbolWindow->SetTarget(this);	
	}
	return fStockSymbolWindow;
}

void
App::MessageReceived(BMessage *message) {
		
	switch (message->what) {
		case kUpdateSymbolMessage: 
			HandleUpdate(message);
			break;
		
		case kHideSearchWindowMessaage:
			fStockSymbolWindow = NULL;
			break;
			
		case kAddSymbolButtonPressedMessage:
			message->PrintToStream();
			break;
		
		case kShowSearchWindowMessage:
			if (SymbolWindow()->IsHidden() || SymbolWindow()->IsMinimized()) {
				SymbolWindow()->SetStockSymbols(fSymbolList);
				SymbolWindow()->Show();				
			}
			break;
		
		default:
			message->PrintToStream();
			break;
	}
}

void 
App::HandleUpdate(BMessage *message) {
	BMessage symbolMessage;
			
	if (message->FindMessage("Symbols", &symbolMessage) == B_OK) {	
		char *name;
		uint32 type;
		int32 count;
				
		if (fSymbolList == NULL) {
			fSymbolList = new BList();
		}
				
		for (int32 i = 0; symbolMessage.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
			BMessage currentMessage;
			if (symbolMessage.FindMessage(name, &currentMessage) == B_OK) {
				StockSymbol *symbol = new StockSymbol(currentMessage);
				SymbolListItem* symbolListItem = new SymbolListItem(symbol);
				fSymbolList->AddItem(symbolListItem);
			}
		}
	}
}

int main() {
	App app;
	app.Run();
	return 0; 	
}
