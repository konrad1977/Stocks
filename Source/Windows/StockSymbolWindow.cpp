/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockSymbolWindow.h"
#include "StockRequester.h"
#include <ColumnListView.h>
#include "MessageConstants.h"
#include "SymbolListItem.h"
#include "StockSymbol.h"

#include <stdio.h>


StockSymbolWindow::StockSymbolWindow(BRect rect)
	:BWindow(rect, "MainWindow", B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0) {
	
	fSymbolListView = new BListView(Bounds(), "Symbols", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	fScrollView = new BScrollView("ScrollView", fSymbolListView, B_FOLLOW_ALL, B_FRAME_EVENTS, true, true);
	AddChild(fScrollView);
	
	fStockRequester = new StockRequester(this);
	fStockRequester->DownloadSymbols();	
}

StockSymbolWindow::~StockSymbolWindow() {
}

void
StockSymbolWindow::MessageReceived(BMessage *message) {
	
	switch (message->what) {
		case kUpdateSymbolMessage: {
			printf("Got data\n");
			BMessage symbolMessage;
			
			if (message->FindMessage("Symbols", &symbolMessage) == B_OK) {
				char *name;
				uint32 type;
				int32 count;
				
				for (int32 i = 0; symbolMessage.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
					BMessage currentMessage;
					if (symbolMessage.FindMessage(name, &currentMessage) == B_OK) {
						StockSymbol *symbol = new StockSymbol(currentMessage);
						fSymbolListView->AddItem(new SymbolListItem(symbol));
					}
				}
			}	
			//StockSymbol *symbol = new StockSymbol(symbolMessage);
			//symbol->PrintToStream();
			//printf("Symbol %s", symbol->name.String());
			//fSymbolListView->AddItem(new SymbolListItem(symbol));
		}
		break;
		
		default:
			break;	
	}
}
