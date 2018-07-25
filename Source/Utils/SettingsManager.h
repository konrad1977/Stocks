/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H


#include <SupportDefs.h>
#include <Message.h>

class BList;
class SettingsManager {
public:
	SettingsManager();
	~SettingsManager();

	bool HasSymbol(const char *symbol);
	void RemoveSymbol(const char *symbol);
	void AddSymbol(const char *symbol);
	
	void SaveSymbols(BList *list);
	BList *LoadSymbols();
	
private:
	int32 	IndexOf(const char *symbol);
	
	status_t LoadSettings(BMessage &message);
	status_t SaveSettings(BMessage message);
	char *fFileName;
	BList *fCurrentLoadedSymbols;
};


#endif // _H
