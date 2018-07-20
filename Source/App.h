/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef APP_H
#define APP_H


#include <SupportDefs.h>
#include <Application.h>
#include "StockSymbolWindow.h"

class SettingsManager;
class StockRequester;
class StockSymbolWindow;
class MainWindow;
class App: public BApplication {
public:
	App(void);		
	~App();
	
	virtual void MessageReceived(BMessage *message);
		
private:
			void HandleUpdate(BMessage *message);
			void SaveSettings(BMessage *message);
			void AddToPortfolio(const char *symbol);
			bool HasSymbol(const char *symbol);


	StockSymbolWindow *SymbolWindow();
	
	MainWindow *fWindow;
	
	StockRequester *fStockRequester;
	StockSymbolWindow *fStockSymbolWindow;
	SettingsManager *fSettingsManager;
	BList *fSymbolList;
	BList *fCurrentSymbols;
};


#endif // _H
