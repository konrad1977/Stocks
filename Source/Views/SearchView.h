/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H


#include <SupportDefs.h>
#include <View.h>

class BTextControl;
class BButton;
class BMessenger;
class BStringView;
class SearchView: public BView {
public:
	SearchView();
	~SearchView();
		
	virtual void AllAttached();
	virtual void MessageReceived(BMessage *message);
	virtual void AttachedToWindow();
			void SetTarget(BHandler *handler);
			void SetNumberOfHits(int32 hits);
private:
	BTextControl *TextControl();
			void InitLayout();
			
	BTextControl 	*fSearchTextControl;
	BStringView  	*fHitsView;
	BMessenger 		*fMessenger;	
};


#endif // _H
