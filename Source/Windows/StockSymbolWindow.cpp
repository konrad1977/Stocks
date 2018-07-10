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
#include "StockListExtendedView.h"

#include <stdio.h>
#include <List.h>


StockSymbolWindow::StockSymbolWindow(BRect rect)
	:BWindow(rect, "Stock symbols", B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
	,fStockRequester(NULL)
	,fSearchView(NULL)
	,fStockListExtendedView(NULL)
	,fSymbolListView(NULL)
	,fStockSymbolListItems(NULL)
	,fCurrentFilter(NULL)
	,fHasFilter(false) {
	
	InitLayout();
	
	fStockRequester = new StockRequester(this);
	fStockRequester->DownloadSymbols();	
}

StockSymbolWindow::~StockSymbolWindow() {
	if (fCurrentFilter) {
		fCurrentFilter->MakeEmpty();
	}
	delete fCurrentFilter;
	delete fStockRequester;
}

void
StockSymbolWindow::InitLayout() {
	
	BRect frame = Bounds();
	float height = frame.Height();
	frame.bottom = 46;
	
	fSearchView = new SearchView(frame);
	fSearchView->SetTarget(this);
	AddChild(fSearchView);
	
	const float extendedHeight = 80.0;
	
	frame.top = frame.bottom;
	frame.bottom = height - (B_H_SCROLL_BAR_HEIGHT + extendedHeight);
	frame.right -=B_H_SCROLL_BAR_HEIGHT;
	
	fSymbolListView = new BListView(frame, "Symbols", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	fSymbolListView->SetSelectionMessage(new BMessage(kSymbolListSelectionChanged));
	fSymbolListView->SetTarget(this);
	
	fScrollView = new BScrollView("ScrollView", fSymbolListView, B_FOLLOW_ALL, 0, true, true);
	AddChild(fScrollView);
	
	frame.right = Bounds().right;
	frame.top = Bounds().bottom - extendedHeight;
	frame.bottom = Bounds().bottom;
	fStockListExtendedView = new StockListExtendedView(frame);
	AddChild(fStockListExtendedView);
}

BList *
StockSymbolWindow::Filtered(BString filter) {
	
	if (fStockSymbolListItems == NULL || filter.Length() == 0) {
		return NULL;
	}
	
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
StockSymbolWindow::HandleSelection(BMessage *message) {
	
	int32 index;
	if (message->FindInt32("index", &index) == B_OK) {
		printf("Index found %d\n", index);
		
		SymbolListItem *listItem = static_cast<SymbolListItem *>(fSymbolListView->ItemAt(index));
		if (listItem == NULL) {
			return;
		}
		
		const char *symbol = listItem->CurrentStockSymbol()->symbol.String();
		
		if (strlen(symbol) == 0 || symbol == NULL) 			
			return;

		fStockRequester->RequestStockInformation(symbol);
	}
}
			
void 
StockSymbolWindow::HandleUpdate(BMessage *message) {
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
			
void 
StockSymbolWindow::HandleSearch(BMessage *message) {
	BString searchString;
	if (message->FindString("searchText", &searchString) == B_OK ) {
		ApplyFilter(searchString);
	}
}

void
StockSymbolWindow::MessageReceived(BMessage *message) {
	
	switch (message->what) {
		
		case kUpdateCompanyMessage:
			message->PrintToStream();
			break;

		case kSymbolListSelectionChanged: 
			HandleSelection(message);
			break;
		
		case kSearchTextChangedMessage: 
			HandleSearch(message);
			break;
		
		case kUpdateSymbolMessage: 
			HandleUpdate(message);
			break;
			
		default:
			break;	
	}
}
