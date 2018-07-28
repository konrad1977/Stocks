/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef QUOTELISTITEM_H
#define QUOTELISTITEM_H

#include <SupportDefs.h>
#include <ListItem.h>
#include "Quote.h"

enum QuoteSize {
	SMALL = 0,
	NORMAL,
	LARGE
};

class QuoteListItem : public BListItem {
public:
	QuoteListItem(Quote *quote, bool replicant, QuoteSize quoteSize = SMALL);
	~QuoteListItem();
	
	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);
	
			void SetQuoteItemSize(QuoteSize size);
	Quote  *CurrentQuoteItem();
	
private:
			rgb_color TextColor(); 
			rgb_color BackgroundColor();
			
			void DrawSmallItem(BView *view, BRect frame);
			void DrawNormalItem(BView *view, BRect frame);
			void DrawLargeItem(BView *view, BRect frame);
			
			void DrawCompanyName(BView *view, BRect frame);
			void DrawChangePercent(BView *view, BRect frame);
			void DrawChangeDollar(BView *view, BRect frame);
			void DrawLatestPrice(BView *view, BRect frame);
			
	Quote *fQuote;
	QuoteSize fQuoteSize;
	bool fIsReplicant;
};


#endif // _H
