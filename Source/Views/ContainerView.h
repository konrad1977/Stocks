/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 
#ifndef CONTAINERVIEW_H
#define CONTAINERVIEW_H

#include <SupportDefs.h>
#include <interface/View.h>
#include "QuoteListItem.h"

class SettingsManager;
class StockRequester;
class Portfolio;
class BDragger;
class BMessageRunner;
class BListView;
class BList;
class ContainerView : public BView {
public:
	ContainerView(Portfolio *portfolio);
	ContainerView(BMessage *archive);
	~ContainerView();
	
	virtual void		AttachedToWindow();
	virtual void 		MessageReceived(BMessage *message);
	virtual status_t	Archive(BMessage* into, bool deep = true) const;
	static BArchivable*	Instantiate(BMessage* archive);
			status_t	SaveState(BMessage* into, bool deep = true) const;
		
			void SetTarget(BHandler *handler);
			void RequestData();	
private:

	StockRequester	*Requester();	
	SettingsManager *CurrentSettingsManager();
	
			void InitAutoUpdate();
			void ShowAlert(const char *title, const char *message);
			void SendEmptyListMessage();
			void UpdateQuoteItemSizes(QuoteSize size);
			void UpdateItemWithQuote(Quote *quote);
			void HandleQuotes(BMessage message);
			void LoadSavedData();
			void SetupViews();
			void StopActiveRequest();
			void DownloadData();
			void RemoveSelectedListItem();
	static int32 DownloadDataFunc(void *cookie);	
		
	BDragger 		*fDragger;	
	BListView 		*fQuoteListView;
	BList 			*fCurrentSymbols;

	BMessenger		*fMessenger;
	BMessageRunner	*fAutoUpdateRunner;
	StockRequester  *fStockRequester;
	Portfolio 		*fPortfolio;
	SettingsManager *fSettingsManager;
	
	thread_id		fDownloadThread;
	bool 			fIsReplicant;
};

#endif // _H
