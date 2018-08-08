/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PORTFOLIOMANAGERWINDOW_H
#define PORTFOLIOMANAGERWINDOW_H


#include <SupportDefs.h>
#include <interface/Window.h>

class BMenuBar;
class PortfolioWindow;
class PortfolioManager;
class PortfolioManagerWindow : public BWindow {
public:
	PortfolioManagerWindow(PortfolioManager *manager);
	~PortfolioManagerWindow();
	
	virtual void MessageReceived(BMessage *message);
	
private:
			void InitLayout();
			void HandleNewPortfolioMessage(BMessage &message);
			
	PortfolioWindow 	*fPortfolioWindow;
	PortfolioManager 	*fPortfolioManager;
	BMenuBar 			*fMenuBar;
};

#endif // _H
