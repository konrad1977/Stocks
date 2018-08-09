/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PORTFOLIOMANAGERWINDOW_H
#define PORTFOLIOMANAGERWINDOW_H


#include <SupportDefs.h>
#include <interface/Window.h>

class BListView;
class BMenuBar;
class PortfolioWindow;
class PortfolioManager;
class PortfolioManagerWindow : public BWindow {
public:
	PortfolioManagerWindow();
	~PortfolioManagerWindow();
	
	virtual void MessageReceived(BMessage *message);
	
private:
			void InitLayout();
			void ReloadPortfolios();
			
			void HandleNewPortfolioMessage(BMessage &message);
			void HandleAlreadyExist(BString previousName);
			
	PortfolioWindow 	*fPortfolioWindow;
	PortfolioManager 	*fPortfolioManager;
	BMenuBar 			*fMenuBar;
	BListView 			*fListView;
};

#endif // _H
