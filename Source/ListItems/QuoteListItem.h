/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef QUOTELISTITEM_H
#define QUOTELISTITEM_H


#include <SupportDefs.h>
#include <ListItem.h>
#include "Quote.h"

class QuoteListItem : public BListItem {
public:
	QuoteListItem(Quote *quote, bool replicant);
	~QuoteListItem();
	
	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);
								
private:
			rgb_color TextColor(); 
			rgb_color BackgroundColor();
			
			void DrawCompanyName(BView *view, BRect frame);
			void DrawChangePercent(BView *view, BRect frame);
			void DrawChangeDollar(BView *view, BRect frame);
			void DrawLatestPrice(BView *view, BRect frame);
			
	Quote *fQuote;
	bool fIsReplicant;
};


#endif // _H
