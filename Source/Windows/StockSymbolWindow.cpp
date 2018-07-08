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
#include "SearchView.h"
#include <stdio.h>


StockSymbolWindow::StockSymbolWindow(BRect rect)
	:BWindow(rect, "Stock symbols", B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0) {
	
	BRect frame = Bounds();
	float height = frame.Height();
	frame.bottom = 100;
	
	fSearchView = new SearchView(frame);
	AddChild(fSearchView);
	
	frame.top = 100;
	frame.bottom = height - B_H_SCROLL_BAR_HEIGHT;
	frame.right -=B_H_SCROLL_BAR_HEIGHT;
	
	fSymbolListView = new BListView(frame, "Symbols", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	fScrollView = new BScrollView("ScrollView", fSymbolListView, B_FOLLOW_ALL, 0, true, true);
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
		}
		break;
		
		default:
			break;	
	}
}
