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
class ListView;
class BList;
class ContainerView : public BView {
public:
	ContainerView(Portfolio *portfolio);
	ContainerView(BMessage *archive);
	~ContainerView();

	virtual void		AttachedToWindow();
	virtual void 		MessageReceived(BMessage *message);
	virtual void		MouseDown(BPoint point);
	virtual void 		MouseUp(BPoint point);
	
	static BArchivable*	Instantiate(BMessage* archive);

	virtual status_t	Archive(BMessage* into, bool deep = true) const;
			status_t	SaveState(BMessage* into, bool deep = true) const;

			void SetTarget(BHandler *handler);
			void RequestData();
private:

	StockRequester	*Requester();
	SettingsManager *CurrentSettingsManager();

			void EmptyQuoteItems();
			void ResizeToFit();
			void InitAutoUpdate();
			void ShowAlert(const char *title, const char *message);
			void SendEmptyListMessage();
			void UpdateQuoteItemType(QuoteType type);
			void UpdateItemWithQuote(Quote *quote);
			void HandleQuotes(BMessage message);
			void LoadSavedData();
			void SetupViews();
			void SetupViewAsReplicant();
			void StopActiveRequest();
			void DownloadData();
			void RemoveSelectedListItem();
	
			void StartNetworkMonitoring();
			bool IsConnected();
			
	static int32 DownloadDataFunc(void *cookie);

	BDragger 		*fDragger;
	ListView 		*fQuoteListView;
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
