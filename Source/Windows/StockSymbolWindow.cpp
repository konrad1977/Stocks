/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockSymbolWindow.h"
#include "StockRequester.h"
#include "MessageConstants.h"
#include "SymbolListItem.h"
#include "StockSymbol.h"
#include "SearchView.h"

#include <stdio.h>
#include <List.h>


StockSymbolWindow::StockSymbolWindow(BRect rect)
	:BWindow(rect, "Stock symbols", B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
	,fStockSymbolListItems(NULL)
	,fHasFilter(false) {
	
	BRect frame = Bounds();
	float height = frame.Height();
	frame.bottom = 46;
	
	fSearchView = new SearchView(frame);
	fSearchView->SetTarget(this);
	AddChild(fSearchView);
	
	frame.top = 46;
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

BList *
StockSymbolWindow::Filtered(BString filter) {
	
	if (fStockSymbolListItems == NULL || filter.Length() == 0) {
		return NULL;
	}
	
	printf("Filter == %s\n", filter);
	
	if (fCurrentFilter) {
		fCurrentFilter->MakeEmpty();
	}
	
	delete fCurrentFilter;
	fCurrentFilter = new BList();
	
	const int32 count = fStockSymbolListItems->CountItems();
	
	for (int32 i = 0; i<count; i++) {
		SymbolListItem *symbol = (SymbolListItem*)fStockSymbolListItems->ItemAt(i);
		if (symbol != NULL) {
			if (symbol->CurrentStockSymbol()->name.ToLower().FindFirst(filter.ToLower()) != B_ERROR ){
				fCurrentFilter->AddItem(symbol);
			} else if (symbol->CurrentStockSymbol()->symbol.ToLower().FindFirst(filter.ToLower()) != B_ERROR ){
				fCurrentFilter->AddItem(symbol);
			} 
		}
	}
	return fCurrentFilter;
}

void 
StockSymbolWindow::SetItems(BList *listItems) {
	
	if (listItems == NULL) 
		return;
		
	const int32 items = listItems->CountItems();
	
	fSymbolListView->MakeEmpty();
	fSymbolListView->AddList(listItems);
}

void 
StockSymbolWindow::ApplyFilter(BString filter) {
	if (filter.Length() > 2) {
		BList *filtered = Filtered(filter);
		SetItems(filtered);
		fHasFilter = true;
	} else if (fHasFilter == true) {
		SetItems(fStockSymbolListItems);
		fHasFilter = false;
	}
}

void
StockSymbolWindow::MessageReceived(BMessage *message) {
	
	switch (message->what) {
		case kSearchTextChangedMessage: {
			BString searchString;
			if (message->FindString("searchText", &searchString) == B_OK ) {
				ApplyFilter(searchString);
			}
		}
		break;
		case kUpdateSymbolMessage: {
			BMessage symbolMessage;
			
			if (message->FindMessage("Symbols", &symbolMessage) == B_OK) {
				char *name;
				uint32 type;
				int32 count;
				
				delete fStockSymbolListItems;
				fStockSymbolListItems = new BList();
				
				for (int32 i = 0; symbolMessage.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
					BMessage currentMessage;
					if (symbolMessage.FindMessage(name, &currentMessage) == B_OK) {
						StockSymbol *symbol = new StockSymbol(currentMessage);
						SymbolListItem* symbolListItem = new SymbolListItem(symbol);
						fStockSymbolListItems->AddItem(symbolListItem);
					}
				}
			}
			SetItems(fStockSymbolListItems);
		}
		break;
		
		default:
			break;	
	}
}
