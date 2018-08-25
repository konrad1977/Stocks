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


#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

#include <Alert.h>
#include <Catalog.h>
#include <ListView.h>
#include <ScrollView.h>
#include <stdio.h>
#include <List.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "StockSymbolWindow"

StockSymbolWindow::StockSymbolWindow()
	:BWindow(BRect(1,1,1,1), B_TRANSLATE("Find stocks..."), B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
	,fSearchView(NULL)
	,fStockListExtendedView(NULL)
	,fSymbolListView(NULL)
	,fStockSymbolListItems(NULL)
	,fCurrentFilter(NULL)
	,fMessenger(NULL)
	,fHasFilter(false)  {

	InitLayout();
	CenterOnScreen();
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
	printf("HasSymbolInPortfolio :: Todo:\n");
	return false;
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
	
	fSearchView = new SearchView();
	fSearchView->SetTarget(this);

	fSymbolListView = new BListView("Symbols", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	fSymbolListView->SetSelectionMessage(new BMessage(kSymbolListSelectionChanged));
	fSymbolListView->SetTarget(this);
	fSymbolListView->SetExplicitMinSize(BSize(400, 300));
	
	BScrollView *scrollView = new BScrollView("ScrollView", fSymbolListView, 0, false, true);

	fStockListExtendedView = new StockListExtendedView();
	fStockListExtendedView->SetTarget(this);	
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 10)
		.Add(fSearchView)
		.Add(scrollView)
		.Add(fStockListExtendedView)
	.End();	
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
			if (symbol->CurrentStockSymbol().name.ToLower().FindFirst(filter.ToLower()) != B_ERROR ){
				fCurrentFilter->AddItem(symbol);
			} else if (symbol->CurrentStockSymbol().symbol.ToLower().FindFirst(filter.ToLower()) != B_ERROR ){
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
	return listItem->CurrentStockSymbol().symbol.String();
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
