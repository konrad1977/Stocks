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
class SearchView: public BView {
public:
	SearchView(BRect rect);
	~SearchView();
	
	virtual void MessageReceived(BMessage *message);
	virtual void AttachedToWindow();
			void SetTarget(BHandler *handler);
private:
	
	BTextControl *TextControl();
	
	void _LayoutChildren();

	BTextControl *fSearchTextControl;
	BButton *fSearchButton;	
	BMessenger *fMessenger;	
};


#endif // _H
