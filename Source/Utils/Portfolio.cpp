/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Portfolio.h"

#include "SettingsManager.h"
#include "Constants.h"

#include <List.h>
#include <Messenger.h>
#include <Handler.h>
#include <stdio.h>
#include <String.h>

Portfolio::Portfolio()
	:fMessenger(NULL)
	,fSettingsManager(NULL)
	,fCurrentSymbols(NULL) {
	
	fSettingsManager = new SettingsManager();
	fCurrentSymbols = new BList();
	Load();
}

Portfolio::~Portfolio() {
	delete fMessenger;
	delete fSettingsManager;
	delete fCurrentSymbols;
}

BList*
Portfolio::CurrentSymbols() {
	Load();
	return fCurrentSymbols;
}

void Portfolio::Add(const char *symbol) {
	fSettingsManager->AddSymbol(symbol);
	NotifyAdd(symbol);
}
	
void Portfolio::Remove(const char *symbol) {
	fSettingsManager->RemoveSymbol(symbol);
	NotifyRemove(symbol);
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

void
Portfolio::Load() {
	fCurrentSymbols->MakeEmpty();
	fCurrentSymbols = fSettingsManager->LoadSymbols();
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