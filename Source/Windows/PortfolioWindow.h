/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PORTFOLIOWINDOW_H
#define PORTFOLIOWINDOW_H

#include <SupportDefs.h>
#include <interface/Window.h>

class BTextControl;
class BHandler;
class BMessenger;
class PortfolioWindow : public BWindow {
public:
	PortfolioWindow(BHandler *handler);
	~PortfolioWindow();
	
			void SetAlreadyExistingName(BString name);
			
	virtual void MessageReceived(BMessage *message);
	virtual bool QuitRequested();
	
private:
			void InitLayout();
	BTextControl *TextControl();
			
	BMessenger		*fMessenger;
	BTextControl 	*fNameControl;
};

#endif // _H
