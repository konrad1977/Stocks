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
	void SetTransparency(uint8 transparency);
	uint8 Transparency();

	void SetRefreshRate(uint8 seconds);
	uint8 RefreshRate();
	
	void SetQuoteSize(QuoteSize size);
	QuoteSize CurrentQuoteSize();

	void StartMonitoring(BHandler *handler);
	
	bool HasSymbol(const char *symbol);
	void RemoveSymbol(const char *symbol);
	void AddSymbol(const char *symbol);
	
	
	void SaveSymbols(BList *list);
	BList *LoadSymbols();

	status_t LoadSettings(BMessage &message);
	status_t SaveSettings(BMessage message);	
	
private:

	const char* SavePath() const;
	
	int32 	IndexOf(const char *symbol);
	void 	SaveWithLock(BMessage *message);
	
	char 	*fFileName;
	BLocker *fLocker;
};

#endif // _H
