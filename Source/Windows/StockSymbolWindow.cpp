/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockSymbolWindow.h"
#include "StockRequester.h"
#include "Constants.h"
#include "SymbolListItem.h"
#include "StockSymbol.h"
#include "SearchView.h"
#include "StockListExtendedView.h"
#include "Company.h"
#include "Quote.h"
#include "SettingsManager.h"

#include <Alert.h>
#include <Catalog.h>
#include <ListView.h>
#include <ScrollView.h>
#include <stdio.h>
#include <List.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "StockSymbolWindow"

StockSymbolWindow::StockSymbolWindow()
	:BWindow(BRect(200,200,900,720), B_TRANSLATE("Find stocks..."), B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
	,fSearchView(NULL)
	,fStockListExtendedView(NULL)
	,fSymbolListView(NULL)
	,fScrollView(NULL)
	,fStockSymbolListItems(NULL)
	,fCurrentFilter(NULL)
	,fMessenger(NULL)
	,fHasFilter(false)  {
	
	InitLayout();	
}

StockSymbolWindow::~StockSymbolWindow() {	
	if (fCurrentFilter) {
		fCurrentFilter->MakeEmpty();
	}
	delete fMessenger;
	delete fCurrentFilter;
}

bool
StockSymbolWindow::HasSymbolInPortfolio(const char *symbol) {
	SettingsManager *manager = new SettingsManager();
	Lock();
	bool hasSymbol = manager->HasSymbol(symbol);
	Unlock();
	delete manager;
	return hasSymbol;
}

void
StockSymbolWindow::SetTarget(BHandler *handler) {
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
StockSymbolWindow::SetStockSymbols(BList *symbols) {
	fStockSymbolListItems = symbols;
	SetItems(fStockSymbolListItems);
}

bool
StockSymbolWindow::QuitRequested() {
	if (fMessenger == NULL) {
		return true;
	}
	
	BMessage message(kHideSearchWindowMessaage);
	fMessenger->SendMessage(&message);
	return true;
}

void
StockSymbolWindow::InitLayout() {
	
	BRect frame = Bounds();
	float height = frame.Height();
	frame.bottom = 46;
	
	fSearchView = new SearchView(frame);
	fSearchView->SetTarget(this);
	AddChild(fSearchView);
	
	const float extendedHeight = 160.0;
	
	frame.top = frame.bottom;
	frame.bottom = height - (B_H_SCROLL_BAR_HEIGHT + extendedHeight);
	frame.right -=B_H_SCROLL_BAR_HEIGHT;
	
	fSymbolListView = new BListView(frame, "Symbols", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_FULL_UPDATE_ON_RESIZE | B_WILL_DRAW);
	fSymbolListView->SetSelectionMessage(new BMessage(kSymbolListSelectionChanged));
	fSymbolListView->SetTarget(this);
	
	fScrollView = new BScrollView("ScrollView", fSymbolListView, B_FOLLOW_ALL, 0, true, true);
	AddChild(fScrollView);
	
	frame.right = Bounds().right;
	frame.top = Bounds().bottom - extendedHeight;
	frame.bottom = Bounds().bottom;
	fStockListExtendedView = new StockListExtendedView(frame);
	fStockListExtendedView->SetTarget(this);
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
		SymbolListItem *symbol = (SymbolListItem*)fStockSymbolListItems->ItemAtFast(i);
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
	
	if (fSearchView != NULL) {
		fSearchView->SetNumberOfHits(items);
	}	

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

const char*
StockSymbolWindow::SymbolAtIndex(int32 index) {
	
	if (fSymbolListView == NULL) {
		return NULL;
	}
	
	SymbolListItem *listItem = static_cast<SymbolListItem *>(fSymbolListView->ItemAt(index));
	if (listItem == NULL) {
		return NULL;
	}		
	return listItem->CurrentStockSymbol()->symbol.String();
}

void 
StockSymbolWindow::HandleSelection(BMessage *message) {
	
	int32 index;
	if (message->FindInt32("index", &index) == B_OK) {
		if (const char *symbol = SymbolAtIndex(index)) {
			StockRequester requester(this);
			requester.RequestStockInformation(symbol);			
		}
	}
}

void 
StockSymbolWindow::HandleSearch(BMessage *message) {
	BString searchString;
	if (message->FindString("searchText", &searchString) == B_OK ) {
		fSymbolListView->DeselectAll();
		ApplyFilter(searchString);
	}
}

void 
StockSymbolWindow::HandleQuoteInformation(BMessage *message) {
	BMessage quoteMessage;
	if (message->FindMessage("Quote", &quoteMessage) == B_OK) {
		Quote *quote = new Quote(quoteMessage);
		const char *symbol = quote->symbol.String();
		bool hasQuote = HasSymbolInPortfolio(symbol);
		fStockListExtendedView->SetQuote(quote, hasQuote);
	}
}

void
StockSymbolWindow::HandleCompanyInformation(BMessage *message) {
	BMessage companyMessage;
	if (message->FindMessage("Company", &companyMessage) == B_OK) {
		Company *company = new Company(companyMessage);
		fStockListExtendedView->SetCompany(company);
	}
}

void
StockSymbolWindow::HandleAddToPortfolio(BMessage *message) {
	if (fMessenger) 
		fMessenger->SendMessage(message);
}

void 
StockSymbolWindow::ShowAlert(const char *title, const char *message) {
	BAlert *alert = new BAlert(title, message, "Ok");
	alert->SetType(B_WARNING_ALERT);
	alert->Go();
}

void
StockSymbolWindow::MessageReceived(BMessage *message) {
	
	switch (message->what) {

		case kUpdateFailed: {
			BString description;
			if (message->FindString("message", &description) == B_OK) {
				ShowAlert(B_TRANSLATE("Network error"), description.String());
			} else {
				ShowAlert(B_TRANSLATE("Network error"), B_TRANSLATE("Failed to parse data"));
			}
			break;
		}
		
		case kPortfolioButtonPressedMessage:
			HandleAddToPortfolio(message);
			break;
		
		case kUpdateQuoteMessage:
			HandleQuoteInformation(message);
			break;
		
		case kUpdateCompanyMessage:
			HandleCompanyInformation(message);
			break;

		case kSymbolListSelectionChanged: 
			HandleSelection(message);
			break;
		
		case kSearchTextChangedMessage: 
			HandleSearch(message);
			break;
			
		default:
			BWindow::MessageReceived(message);
			break;	
	}
}
