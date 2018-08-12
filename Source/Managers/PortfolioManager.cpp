/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "PortfolioManager.h"
#include "Portfolio.h"
#include "SettingsManager.h"
#include "Constants.h"

#include <Handler.h>
#include <Messenger.h>
#include <support/List.h>
#include <app/Message.h>

PortfolioManager::PortfolioManager(BHandler *handler) 
	:fSettingsManager(NULL)
	,fPortfolioList(NULL)
	,fMessenger(NULL) 
{	
	fPortfolioList = new BList();	
	fMessenger = new BMessenger(handler);
	fSettingsManager = new SettingsManager();
	
	Load();
}

PortfolioManager::~PortfolioManager() 
{
	delete fSettingsManager;
	delete fMessenger;
	delete fPortfolioList;
}

void 
PortfolioManager::NotifyPortfolioChanges()
{
	BMessage message(kPortfolioManagerChangeMessage);
	fMessenger->SendMessage(&message);
}

bool 
PortfolioManager::AddPortfolio(Portfolio *portfolio) 
{
	if (HasPortfolio(portfolio) == false ) {
		fPortfolioList->AddItem(reinterpret_cast<void*>(portfolio));
		Save();
		return true;
	}
	return false;
}

void 
PortfolioManager::RemovePortfolio(Portfolio *portfolio) 
{
	const int32 items = fPortfolioList->CountItems();
	
	for (int32 i = 0; i<items; i++) {
		Portfolio *item = static_cast<Portfolio*>(fPortfolioList->ItemAtFast(i));

		if (item == NULL) {
			continue;
		}
		
		if (portfolio->IsEqual(*item) == true) {
			fPortfolioList->RemoveItem(i);
			Save();
			delete item;
			break;
		}
	}
}

bool
PortfolioManager::HasPortfolio(Portfolio *portfolio) {
	const int32 items = fPortfolioList->CountItems();
	for (int32 i = 0; i<items; i++) {
		Portfolio *item = static_cast<Portfolio*>(fPortfolioList->ItemAtFast(i));
		if (item == NULL) {
			continue;
		}
		if (portfolio->IsEqual(*item) == true) {
			return true;
		}
	}
	return false;
}

BList *
PortfolioManager::Portfolios() const 
{
	return fPortfolioList;
}

void 
PortfolioManager::PrintToStream() 
{
	const int32 items = fPortfolioList->CountItems();
	
	for (int32 i = 0; i<items; i++) {
		Portfolio *item = static_cast<Portfolio*>(fPortfolioList->ItemAtFast(i));
		if (item == NULL) {
			continue;
		}
		printf("Portfolio: %s\n", item->Name().String());
		item->PrintToStream();
	}
}

status_t 
PortfolioManager::Save() {

	BMessage message;
	const int32 items = fPortfolioList->CountItems();
	for (int32 i = 0; i<items; i++) {
		Portfolio *item = static_cast<Portfolio*>(fPortfolioList->ItemAtFast(i));
		if (item == NULL) {
			continue;
		}
		BMessage portfolioMessage;
		portfolioMessage.AddString("Portfolio", item->Name());
		item->Save(portfolioMessage);	
		
		message.AddMessage("Portfolios", &portfolioMessage);
	}
	
	fSettingsManager->SaveSettings(message);
	NotifyPortfolioChanges();
	
	return B_OK; //TODO
}

status_t 
PortfolioManager::Load() 
{
	BMessage message;
	fSettingsManager->LoadSettings(message);
	
	int32 index = 0;
	BMessage portfolioMsg;
	while ( (message.FindMessage("Portfolios", index, &portfolioMsg) == B_OK )) {
		BString portfolioName;
		if (portfolioMsg.FindString("Portfolio", &portfolioName) == B_OK) {
			Portfolio *portfolio = new Portfolio(portfolioName);
			portfolio->Load(portfolioMsg);
			fPortfolioList->AddItem((void*)portfolio);
		}
		index++;
	}	
	return B_OK; //TODO
}
