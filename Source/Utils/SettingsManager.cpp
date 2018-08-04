/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SettingsManager.h"
#include "Constants.h"

#include <String.h>
#include <Roster.h>
#include <Message.h>
#include <Entry.h>
#include <Path.h>
#include <FindDirectory.h>
#include <File.h>
#include <string.h>
#include <List.h>
#include <stdio.h>

SettingsManager::SettingsManager()
	:fFileName(NULL) {
	fFileName = strdup("Stocks");
}

SettingsManager::~SettingsManager() {
	free(fFileName);
}

void 
SettingsManager::RemoveSymbol(const char *symbol) {
	int32 index = IndexOf(symbol);
	if (index != -1) {
		BList *list = LoadSymbols();
		list->RemoveItem(index);
		SaveSymbols(list);
		delete list;
	}
}

void
SettingsManager::AddSymbol(const char *symbol) {

	if (HasSymbol(symbol)) {
		return;
	}
	
	BList *symbolList = LoadSymbols();
	symbolList->AddItem((void *)symbol);
	SaveSymbols(symbolList);
	delete symbolList;
}

int32 
SettingsManager::IndexOf(const char *symbol) {

	BList *list = LoadSymbols();
	if (list == NULL) {
		delete list;
		return -1;
	}
	
	for(int32 i = 0; i<list->CountItems(); i++) {
		char *sym = (char *)list->ItemAtFast(i);
		if (strcasecmp(sym, symbol) == 0) {
			delete list;
			return i;
		}
	}	
	delete list;
	return -1;
}

void 
SettingsManager::SaveSymbols(BList *list) {
	
	if (list == NULL || list->IsEmpty()) {
		return;
	}	
	
	BMessage previousSave;	
	for (int32 index = 0; index<list->CountItems(); index++) {
		const char *symbol = (const char *)list->ItemAtFast(index);
		if (symbol == NULL) {
			continue;
		}
		BMessage symbolMsg;
		symbolMsg.AddString("Symbol", symbol);
		previousSave.AddMessage("Symbols", &symbolMsg);
	}
	SaveSettings(previousSave);
}

bool
SettingsManager::HasSymbol(const char *symbol) {
	return IndexOf(symbol) != -1;
}

uint8
SettingsManager::Transparency() {	
	uint8 value = 127;
	
	BMessage message;
	LoadSettings(message);
	if (message.FindUInt8("Transparency", &value) != B_OK) {
		return 127;
	}
	return value;
}

void
SettingsManager::SetTransparency(uint8 transparency) {
	BMessage message;
	LoadSettings(message);
	
	if (message.ReplaceUInt8("Transparency", transparency) != B_OK) {
		message.AddUInt8("Transparency", transparency);
	}
	SaveSettings(message);
	message.PrintToStream();	
}

void 
SettingsManager::SetQuoteSize(QuoteSize size) {
	BMessage message;
	LoadSettings(message);
	
	int32 value = int32(size);
	if (message.ReplaceInt32("size", value) != B_OK) {
		message.AddInt32("size", int32(size));
	}
	SaveSettings(message);
	message.PrintToStream();	
}

QuoteSize 
SettingsManager::CurrentQuoteSize() {
	BMessage message;
	LoadSettings(message);
	int32 size = message.FindInt32("size");
	printf("Size %d\n", size); 
	return QuoteSize(size);
}
	
BList *
SettingsManager::LoadSymbols() {

	BMessage message;
	if (LoadSettings(message) != B_OK) {
		return new BList();
	}

	BList *list = new BList();
	
	int32 index = 0;
	BMessage symbolMsg;
	while ( (message.FindMessage("Symbols", index, &symbolMsg) == B_OK )) {
		BString symbolString;
		if (symbolMsg.FindString("Symbol", &symbolString) == B_OK) {
			char *symbol = strdup(symbolString.String());
			list->AddItem((void*)symbol);
		}
		index++;
	}	
	return list;
}

status_t 
SettingsManager::SaveSettings(BMessage message) {
	
	BPath path;
	BFile file;
	
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) != B_OK) {
		return B_ERROR;
	}
	path.Append(fFileName);

	file.SetTo(path.Path(), B_WRITE_ONLY | B_ERASE_FILE | B_CREATE_FILE);
	
	if (file.InitCheck() != B_OK) {
		return B_ERROR;
	}

	if (message.Flatten(&file) != B_OK) {
		return B_ERROR;
	}
	return B_OK;
}

status_t
SettingsManager::LoadSettings(BMessage &message) {

	BPath path;
	BFile file;
	
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) != B_OK) {
		return B_ERROR;
	}
	path.Append(fFileName);
	
	file.SetTo(path.Path(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return B_ERROR;
	}
	
	if (message.Unflatten(&file) != B_OK) {
		return B_ERROR;
	}
	return B_OK;
}