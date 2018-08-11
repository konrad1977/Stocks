/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H


#include <SupportDefs.h>
#include <Message.h>
#include <Path.h>
#include "QuoteListItem.h"

class BList;
class BLocker;
class BMessenger;
class SettingsManager {
public:
	SettingsManager();
	~SettingsManager();
	
	void SetTarget(BHandler *handler);
	
	BMessage* MessageForPortfolio(BString name);
	
	void StartMonitoring(BHandler *handler);
	
	status_t LoadSettings(BMessage &message);
	status_t SaveSettings(BMessage message);	
	
private:

	const char* SavePath() const;
	void 	SaveWithLock(BMessage *message);
	
	char 	*fFileName;
	BLocker *fLocker;
};

#endif // _H
