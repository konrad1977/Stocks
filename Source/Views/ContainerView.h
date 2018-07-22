/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CONTAINERVIEW_H
#define CONTAINERVIEW_H

#include <SupportDefs.h>
#include <View.h>

class StockRequester;
class SettingsManager;
class BDragger;
class BListView;
class BList;
class ContainerView : public BView {
public:
	ContainerView();
	ContainerView(BMessage *archive);
	~ContainerView();
	
	virtual void		AttachedToWindow();
	virtual void 		MessageReceived(BMessage *message);
	virtual status_t	Archive(BMessage* into, bool deep = true) const;
	static BArchivable*	Instantiate(BMessage* archive);
			status_t	SaveState(BMessage* into, bool deep = true) const;
			
			void RequestData();	
private:
		void SetupViews();
		void HandleQuotes(BMessage message);
		StockRequester* Requester();
		
	BDragger *fDragger;	
	BListView *fQuoteListView;
	BList *fCurrentSymbols;

	SettingsManager *fSettingsManager;
	StockRequester  *fStockRequester;
	rgb_color fBackgroundColor;
	
	bool fIsReplicant;
};

#endif // _H
