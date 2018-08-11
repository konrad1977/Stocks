/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
*/
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SupportDefs.h>
#include <Window.h>
#include "QuoteListItem.h"

class BMenuBar;
class BMessenger;
class StockRequester;
class SettingsWindow;
class StockSymbolWindow;
class ContainerView;
class Portfolio;
class MainWindow: public BWindow {
public:
	MainWindow(Portfolio *portfolio);	
	~MainWindow();
	
	virtual void MessageReceived(BMessage *message);
			void SetTarget(BHandler *handler);

private:
			void SendSaveMessage();
			void SetSelectedMenuFromQuoteSize(QuoteSize size);
			void InitQuoteSize();
			void SetupViews();	
			void ShowStockWindow();
			void SendToContainerView(BMessage *message);
				
	SettingsWindow		*CurrentSettingWindow();
	
	BMenuBar 			*fMenuBar;		
	ContainerView 		*fContainerView;
	
	SettingsWindow		*fSettingsWindow;
	
	BMenuItem 			*fRemoveSelected;
	BMenuItem			*fMinimalItem;
	BMenuItem			*fNormalItem;
	BMenuItem			*fExtenededItem;
	BMessenger 			*fMessenger;
	Portfolio			*fPortfolio;
};

#endif
