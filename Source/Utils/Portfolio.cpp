/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Portfolio.h"

#include "SettingsManager.h"
#include "MessageConstants.h"

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
	return fCurrentSymbols;
}

void Portfolio::Add(const char *symbol) {
	fSettingsManager->AddSymbol(symbol);
	Load();
}
	
void Portfolio::Remove(const char *symbol) {
	fSettingsManager->RemoveSymbol(symbol);
	Load();
}

void
Portfolio::NotifyChange() {	
	BMessage message(kPortfolioChangedMessage);
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
	if (removeFromPortfolio) {
		Remove(symbol.String());
	} else {
		Add(symbol.String());
	}
	printf("%s %s\n", symbol, removeFromPortfolio ? "Remove" : "Adding");
}