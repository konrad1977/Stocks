/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SettingsManager.h"
#include "MessageConstants.h"

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
	fFileName = strdup("HaikuStocks");
}

SettingsManager::~SettingsManager() {
	free(fFileName);
}

void 
SettingsManager::SaveSymbols(BList *list) {
	
	if (list == NULL || list->IsEmpty()) {
		return;
	}	
	
	BMessage previousSave;
	//LoadSettings(previousSave);
	
	for (int32 index = 0; index<list->CountItems(); index++) {
		const char *symbol = (const char *)list->ItemAt(index);
		if (symbol == NULL) {
			continue;
		}
		BMessage symbolMsg;
		symbolMsg.AddString("Symbol", symbol);
		previousSave.AddMessage("Symbols", &symbolMsg);
	}
	previousSave.PrintToStream();
	SaveSettings(previousSave);
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
			printf("%s %s\n", __FUNCTION__, symbolString.String());
			char *symbol = strdup(symbolString.String());
			list->AddItem((void*)symbol);
		}
		index++;
	}

	for(int32 i = 0; i<list->CountItems(); i++) {
		char *item = (char *)list->ItemAt(i);
		printf("DEBUG Item %s\n", item);
	}
	
	return list;
}

status_t 
SettingsManager::SaveSettings(BMessage message) {
	
	BPath path;
	BFile file;
	
/*	app_info info;
	be_roster->GetActiveAppInfo(&info);
	message.AddRef("appLocation", &info.ref);*/
	
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