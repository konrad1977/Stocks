/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "MainWindow.h"
#include "StockSymbolWindow.h"
#include "MessageConstants.h"
#include "SymbolListItem.h"
#include "StockRequester.h"
#include "SettingsManager.h"
#include <stdio.h>

const char *kAppSignature = "application/x-vnd.konradsson.HaikuStocks";

App::App(void)
	:BApplication(kAppSignature)
	,fCurrentSymbols(NULL)
	,fSymbolList(NULL)
	,fStockRequester(NULL)
	,fWindow(NULL)
	,fStockSymbolWindow(NULL)
	,fSettingsManager(NULL) { 
	
	fSettingsManager = new SettingsManager();
	fCurrentSymbols	= fSettingsManager->LoadSymbols();
	
	fStockRequester = new StockRequester(this);
	fStockRequester->DownloadSymbols();
	
	fWindow = new MainWindow(BRect(150,150,320,640));
	fWindow->Show();		
}

App::~App() {
	delete fStockRequester;
	delete fCurrentSymbols;
	delete fSettingsManager;
}

StockSymbolWindow *
App::SymbolWindow() {
	if (fStockSymbolWindow == NULL) {		
		fStockSymbolWindow = new StockSymbolWindow();
		fStockSymbolWindow->SetTarget(this);	
	}
	return fStockSymbolWindow;
}

bool 
App::HasSymbol(const char *symbol) {

	if (fCurrentSymbols == NULL)
		return false;
	
	for (int32 i = 0; i<fCurrentSymbols->CountItems(); i++) {
		char *sym = (char *)fCurrentSymbols->ItemAtFast(i);
		if (strcasecmp(sym, symbol) == 0) {
			printf("%s %s\n", sym, symbol);
			return true;
		}
	}
	return false;
}

void 
App::AddToPortfolio(const char *symbol) {
	
	if (symbol == NULL || strlen(symbol) < 1 ) {
		printf("Symbol is null or not long enough\n");
		return;
	}
	
	if (HasSymbol(symbol)) {
		printf("Has item %s\n", symbol);
		return;
	}
	
	fCurrentSymbols->AddItem((void*)symbol);
	fSettingsManager->SaveSymbols(fCurrentSymbols);
	
	fWindow->RequestData();
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
			
		case kAddSymbolButtonPressedMessage: {			
			BString symbol;
			if (message->FindString("symbol", &symbol) != B_OK)
				return;
			char *copy = strdup(symbol);
			AddToPortfolio(copy);
		}
		break;
		
		case kShowSearchWindowMessage:
			if (SymbolWindow()->IsHidden() || SymbolWindow()->IsMinimized()) {
				SymbolWindow()->SetStockSymbols(fSymbolList);
				SymbolWindow()->Show();				
			}
			break;
		
		default:
			//message->PrintToStream();
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
