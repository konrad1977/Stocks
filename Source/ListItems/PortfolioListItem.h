/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PORTFOLIOLISTITEM_H
#define PORTFOLIOLISTITEM_H


#include <SupportDefs.h>
#include <interface/ListItem.h>
#include "ListItemDrawer.h"

class Portfolio;
class ListItemDrawer;
class PortfolioListItem : public BListItem {
public:
	PortfolioListItem(Portfolio *portfolio);
	~PortfolioListItem();	

	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);	
	
private:
			void DrawText(const char *text, DrawItemSettings settings);
			void DrawPortfolioInfo(BRect rect);
			
	Portfolio		*fPortfolio;
	ListItemDrawer 	*fDrawer;
};

#endif // _H
