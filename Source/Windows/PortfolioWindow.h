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
	PortfolioWindow(BHandler *handler, bool rename);
	~PortfolioWindow();
	
			void RenamePortfolio(BString currentName);
			void SetAlreadyExistingName(BString name);
			
	virtual void MessageReceived(BMessage *message);
	virtual bool QuitRequested();
	
private:
			void InitLayout();
			void SendMessage(BMessage message);
	BTextControl *TextControl();
			
	BMessenger		*fMessenger;
	BTextControl 	*fNameControl;
	bool 			fIsRenaming;
};

#endif // _H
