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
	,fSettingsManager(NULL)
	,fTransparency(127)	
	,fRereshInterval(60)
	,fQuoteSize(NORMAL)
{
	
	fSettingsManager = new SettingsManager();
	fCurrentSymbols = new BList();
	if (BMessage *msg = fSettingsManager->MessageForPortfolio(name)) {
		Load(*msg);
	}
}

Portfolio::~Portfolio() 
{
	printf("Portfolio::~Portfolio()\n");
	delete fMessenger;
	delete fSettingsManager;
	delete fCurrentSymbols;
}

BString
Portfolio::Name() 
{
	return fName;
}

BList*
Portfolio::CurrentSymbols() 
{
	return fCurrentSymbols;
}

void Portfolio::Add(const char *symbol) 
{
	if (HasSymbol(symbol) == true) {
		return;
	}

	char *symbolToAdd = strdup(symbol);
	
	fCurrentSymbols->AddItem((void*)symbolToAdd);
	NotifyAdd(symbol);
}
	
void Portfolio::Remove(const char *symbol) 
{
	int32 index = IndexOf(symbol);
	if (index == -1) {
		return;
	}
	fCurrentSymbols->RemoveItem(index);
	NotifyRemove(symbol);
}

void
Portfolio::NotifyUpdates() 
{
	BMessage message(kPortfolioUpdatedSettingsMessage);
	fMessenger->SendMessage(&message);
}

void
Portfolio::NotifyAdd(const char *symbol) 
{	
	BMessage message(kPortfolioAddedSymbolMessage);
	message.AddString("symbol", symbol);
	fMessenger->SendMessage(&message);
}

void
Portfolio::NotifyRemove(const char *symbol) 
{
	BMessage message(kPortfolioRemovedSymbolMessage);
	message.AddString("symbol", symbol);
	fMessenger->SendMessage(&message);
}

void 
Portfolio::SetTarget(BHandler *handler) 
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

uint8 
Portfolio::RefreshRate() const
{
	return fRereshInterval;
}

uint8
Portfolio::Transparency() const
{		
	return fTransparency;
}

QuoteSize 
Portfolio::CurrentQuoteSize() const
{
	return fQuoteSize;
}

void 
Portfolio::SetRefreshRate(uint8 seconds) 
{
	if (fRereshInterval != seconds) {
		fRereshInterval = seconds;
		NotifyUpdates();
	}
}

void
Portfolio::SetTransparency(uint8 transparency) 
{
	if (fTransparency != transparency) {
		fTransparency = transparency;
		NotifyUpdates();
	}
}

void 
Portfolio::SetQuoteSize(QuoteSize size) 
{	
	if (fQuoteSize != size) {
		fQuoteSize = size;
		NotifyUpdates();
	}
}

int32 
Portfolio::IndexOf(const char *symbol) 
{	
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
Portfolio::HasSymbol(const char *symbol) 
{
	return IndexOf(symbol) != -1;
}

void
Portfolio::HandlePortfolioUpdate(BMessage *message) 
{
	BString symbol;
	bool removeFromPortfolio = false;

	if (message->FindString("symbol", &symbol) != B_OK)
		return;
		
	removeFromPortfolio = message->FindBool("removeQuote");
	removeFromPortfolio ? Remove(symbol.String()) : Add(symbol.String());
}

void 
Portfolio::PrintToStream() 
{
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
Portfolio::Save(BMessage& message) 
{
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

	if (message.ReplaceUInt8("Refresh", fRereshInterval) != B_OK) {
		message.AddUInt8("Refresh", fRereshInterval);
	}	
	
	if (message.ReplaceUInt8("Transparency", fTransparency) != B_OK) {
		message.AddUInt8("Transparency", fTransparency);
	}

	uint8 value = uint8(fQuoteSize);
	if (message.ReplaceUInt8("size", value) != B_OK) {
		message.AddUInt8("size", uint8(value));
	}		
	return B_OK; //TODO
}

status_t
Portfolio::Load(BMessage& message) 
{	
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

	uint8 size = 1;
	if (message.FindUInt8("size", &size) == B_OK) {
		fQuoteSize = QuoteSize(size);
	} else {
		fQuoteSize = NORMAL;
	}
	
	if (message.FindUInt8("Transparency", &fTransparency) != B_OK) {
		fTransparency = 127;
	}
	
	if (message.FindUInt8("Refresh", &fRereshInterval) != B_OK) {
		fRereshInterval = 60;
	}	
	return B_OK; //TODO
}