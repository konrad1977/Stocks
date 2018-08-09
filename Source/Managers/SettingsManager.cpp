/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SettingsManager.h"
#include "Constants.h"

#include <support/Locker.h>
#include <support/String.h>
#include <support/List.h>

#include <app/Roster.h>
#include <app/Message.h>

#include <storage/Entry.h>
#include <storage/Path.h>
#include <storage/FindDirectory.h>
#include <storage/File.h>
#include <storage/Node.h>
#include <storage/NodeMonitor.h>

#include <string.h>
#include <stdio.h>

SettingsManager::SettingsManager()
	:fFileName(NULL)
	,fLocker(NULL) 
{	
	fFileName = strdup("Stocks");
	fLocker = new BLocker("SettingsLocker");
}

SettingsManager::~SettingsManager() 
{
	free(fFileName);
	delete fLocker;
}

void 
SettingsManager::StartMonitoring(BHandler *handler) 
{
	BNode node;
	node.SetTo( SavePath() );
	
	node_ref ref;
	node.GetNodeRef( &ref );
	
	if (node.InitCheck() == B_OK) {
		watch_node(&ref, B_WATCH_ALL, handler );
	}
}
	
void
SettingsManager::SaveWithLock(BMessage *message)
{
	fLocker->Lock();
	SaveSettings(*message);
	fLocker->Unlock();
}

const char *
SettingsManager::SavePath() const 
{	
	BPath path;
	
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) != B_OK) {
		return NULL;
	}
	
	path.Append(fFileName);
	return path.Path();
}

BMessage* 
SettingsManager::MessageForPortfolio(BString name)
{	
	BMessage loadMessage;
	LoadSettings(loadMessage);
	
	int32 index = 0;
	BMessage portfolioMsg;
	while ( (loadMessage.FindMessage("Portfolios", index, &portfolioMsg) == B_OK )) {
		BString portfolioName;
		if (portfolioMsg.FindString("Portfolio", &portfolioName) == B_OK) {
			if (portfolioName == name) {
				return new BMessage(portfolioMsg);
			}
		}
		index++;
	}	
	return NULL;
}

status_t 
SettingsManager::SaveSettings(BMessage message) 
{	
	BFile file;
	file.SetTo(SavePath(), B_WRITE_ONLY | B_ERASE_FILE | B_CREATE_FILE);
	
	if (file.InitCheck() != B_OK) {
		return B_ERROR;
	}

	if (message.Flatten(&file) != B_OK) {
		return B_ERROR;
	}
	return B_OK;
}

status_t
SettingsManager::LoadSettings(BMessage &message) 
{	
	BFile file;
	
	file.SetTo(SavePath(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return B_ERROR;
	}
	
	if (message.Unflatten(&file) != B_OK) {
		return B_ERROR;
	}
	return B_OK;
}