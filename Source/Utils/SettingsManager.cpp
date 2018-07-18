/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SettingsManager.h"

#include <Message.h>
#include <Entry.h>
#include <Path.h>
#include <FindDirectory.h>
#include <File.h>
#include <string.h>
#include <List.h>

SettingsManager::SettingsManager(const char *fileName)
	:fFileName(NULL) {
	fFileName = strdup(fileName);
}

SettingsManager::~SettingsManager() {
	free(fFileName);
}

void 
SettingsManager::SaveSymbols(BList *list) {

}
	
BList *
SettingsManager::LoadSymbols() const {
	return NULL;
}

status_t 
SettingsManager::SaveSettings() {
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