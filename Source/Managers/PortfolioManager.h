/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PORTFOLIOMANAGER_H
#define PORTFOLIOMANAGER_H

#include <SupportDefs.h>

class BHandler;
class BMessenger;
class BList;

class SettingsManager;
class Portfolio;
class PortfolioManager {
public:
	PortfolioManager(BHandler *handler);
	~PortfolioManager();

			bool AddPortfolio(Portfolio *portfolio);
			void RemovePortfolio(Portfolio *portfolio);
			BList *Portfolios() const;
	status_t 	 Save();
	
			void PrintToStream();
	
private:
			bool HasPortfolio(Portfolio *portfolio);	
	status_t 	 Load();
	
	SettingsManager *fSettingsManager;
	BList 			*fPortfolioList;
	BMessenger 		*fMessenger;
};

#endif // _H
