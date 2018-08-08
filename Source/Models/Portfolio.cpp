/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Portfolio.h"

#include "SettingsManager.h"
#include "Constants.h"

#include <support/List.h>
#include <app/Messenger.h>
#include <app/Handler.h>
#include <posix/stdio.h>

Portfolio::Portfolio(BString name)
	:fName(name)
	,fCurrentSymbols(NULL)
	,fMessenger(NULL)
	,fSettingsManager(NULL) {
	
	fSettingsManager = new SettingsManager();
	fCurrentSymbols = new BList();
	Load();
}

Portfolio::~Portfolio() {
	delete fMessenger;
	delete fSettingsManager;
	delete fCurrentSymbols;
}

BString
Portfolio::Name() {
	return fName;
}

BList*
Portfolio::CurrentSymbols() {
	Load();
	return fCurrentSymbols;
}

void Portfolio::Add(const char *symbol) {
	if (HasSymbol(symbol) == true) {
		return;
	}
	
	fCurrentSymbols->AddItem((void*)symbol);
	//NotifyAdd(symbol);
}
	
void Portfolio::Remove(const char *symbol) {
	int32 index = IndexOf(symbol);
	if (index == -1) {
		return;
	}
	fCurrentSymbols->RemoveItem(index);
	//NotifyRemove(symbol);
}

void
Portfolio::NotifyAdd(const char *symbol) {	
	BMessage message(kPortfolioAddedSymbolMessage);
	message.AddString("symbol", symbol);
	fMessenger->SendMessage(&message);
}

void
Portfolio::NotifyRemove(const char *symbol) {
	BMessage message(kPortfolioRemovedSymbolMessage);
	message.AddString("symbol", symbol);
	fMessenger->SendMessage(&message);
}

void 
Portfolio::SetTarget(BHandler *handler) {
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

int32 
Portfolio::IndexOf(const char *symbol) {
	
	if (fCurrentSymbols == NULL) {
		return -1;
	}
	
	const int32 items = fCurrentSymbols->CountItems();
	for(int32 i = 0; i<items; i++) {
		char *sym = static_cast<char *>(fCurrentSymbols->ItemAtFast(i));
		if (strcasecmp(sym, symbol) == 0) {
			return i;
		}
	}	
	return -1;
}
	
bool 
Portfolio::HasSymbol(const char *symbol) {
	return IndexOf(symbol) != -1;
}

void
Portfolio::Load() {
	//fCurrentSymbols->MakeEmpty();
	//fCurrentSymbols = fSettingsManager->LoadSymbols();
}

void
Portfolio::HandlePortfolioUpdate(BMessage *message) {

	BString symbol;
	bool removeFromPortfolio = false;

	if (message->FindString("symbol", &symbol) != B_OK)
		return;
		
	removeFromPortfolio = message->FindBool("removeQuote");
	removeFromPortfolio ? Remove(symbol.String()) : Add(symbol.String());
}

void 
Portfolio::PrintToStream() {
	const int32 items = fCurrentSymbols->CountItems();
	for (int32 i = 0; i<items; i++) {
		const char *symbol = static_cast<const char *>(fCurrentSymbols->ItemAtFast(i));
		if (symbol == NULL) {
			continue;
		}
		printf("Symbol %s\n", symbol);
	}
}

status_t 	
Portfolio::Save(BMessage& message) {

	const int32 items = fCurrentSymbols->CountItems();
	for (int32 i = 0; i<items; i++) {
		const char *symbol = static_cast<const char *>(fCurrentSymbols->ItemAtFast(i));
		if (symbol == NULL) {
			continue;
		}
		BMessage symbolMsg;
		symbolMsg.AddString("Symbol", symbol);
		message.AddMessage("Symbols", &symbolMsg);
	}
	return B_OK; //TODO
}

status_t
Portfolio::Load(BMessage& message) {
	
	if (fCurrentSymbols == NULL) {
		return B_ERROR;
	}
	
	int32 index = 0;
	BMessage symbolMsg;
	while ( (message.FindMessage("Symbols", index, &symbolMsg) == B_OK )) {
		BString symbolString;
		if (symbolMsg.FindString("Symbol", &symbolString) == B_OK) {
			char *symbol = strdup(symbolString.String());
			fCurrentSymbols->AddItem((void*)symbol);
		}
		index++;
	}
	return B_OK; //TODO
}