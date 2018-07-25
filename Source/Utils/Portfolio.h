/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <SupportDefs.h>

class BList;
class BHandler;
class BMessenger;
class BMessage;
class SettingsManager;
class Portfolio {
public:
	Portfolio();
	~Portfolio();

	void SetTarget(BHandler *handler);
	void Add(const char *symbol);
	void Remove(const char *symbol);
	
	void HandlePortfolioUpdate(BMessage *message);
	BList *CurrentSymbols();
	
private:

	void NotifyChange();
	void Load();
	
	BList 			*fCurrentSymbols;
	BMessenger 		*fMessenger;
	SettingsManager *fSettingsManager;
};


#endif // _H
