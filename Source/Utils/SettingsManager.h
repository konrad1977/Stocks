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
	SettingsManager(const char *fileName);
	~SettingsManager();

	void SaveSymbols(BList *list);
	BList *LoadSymbols() const;
	
private:

	status_t LoadSettings(BMessage &message);
	status_t SaveSettings();

	char *fFileName;
};


#endif // _H
