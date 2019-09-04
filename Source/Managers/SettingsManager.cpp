/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SettingsManager.h"
#include "Constants.h"

#include <support/Locker.h>

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
#include <stdlib.h>

SettingsManager::SettingsManager()
	:fLocker(nullptr)
{
	fFileName = BString("Stocks");
	fLocker = new BLocker("SettingsLocker");
}

SettingsManager::~SettingsManager()
{
	delete fLocker;
}

status_t
SettingsManager::StartMonitoring(BHandler *handler)
{
    BPath path;
    status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	if (status != B_OK) {
		return status;
    }

    status = path.Append(fFileName.String());
    if (status != B_OK) {
        return status;
    }

	BNode node;
	status = node.SetTo(path.Path());

    if (status != B_OK) {
        return status;
    }

	node_ref ref;
	node.GetNodeRef( &ref );

    status = node.InitCheck();
    if (status != B_OK) {
        return status;
    }

    watch_node(&ref, B_WATCH_ALL, handler );
    return B_OK;
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

	path.Append(fFileName.String());
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
    BPath path;
    status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	if (status != B_OK) {
		return status;
    }

    status = path.Append(fFileName.String());
    if (status != B_OK) {
        return status;
    }

	BFile file;
    status = file.SetTo(path.Path(), B_WRITE_ONLY | B_ERASE_FILE | B_CREATE_FILE);
    if (status != B_OK) {
        return status;
    }

    status = file.InitCheck();
    if (status != B_OK) {
        return status;
    }

    return message.Flatten(&file);
}

status_t
SettingsManager::LoadSettings(BMessage &message)
{
    BPath path;
    status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	if (status != B_OK) {
		return status;
    }

    status = path.Append(fFileName.String());
    if (status != B_OK) {
        return status;
    }

	BFile file;
    status = file.SetTo(path.Path(), B_READ_ONLY);
    if (status != B_OK) {
        return status;
    }

    status = message.Unflatten(&file);
	if (status != B_OK) {
		return status;
	}
	return B_OK;
}
