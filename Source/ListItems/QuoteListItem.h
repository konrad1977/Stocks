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

class ListItemDrawer;
class QuoteListItem : public BListItem {
public:
	QuoteListItem(Quote *quote, bool replicant, QuoteSize quoteSize = NORMAL);
	~QuoteListItem();
	
	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);
	
			void SetQuoteItemSize(QuoteSize size);
			void SetQuote(Quote *quote);
			
	Quote  *CurrentQuoteItem();
	
private:
			void DrawSmallItem(BView *view, BRect frame);
			void DrawNormalItem(BView *view, BRect frame);
			void DrawLargeItem(BView *view, BRect frame);
			
			void DrawSymbol(BView *view, BRect frame);
			void DrawMarket(BView *view, BRect frame);
			void DrawCompanyName(BView *view, BRect frame);
			void DrawChangePercent(BView *view, BRect frame);
			void DrawChangeDollar(BView *view, BRect frame);
			void DrawLatestPrice(BView *view, BRect frame);
			
	Quote *fQuote;
	QuoteSize fQuoteSize;
	ListItemDrawer *fDrawer;
	bool fIsReplicant;
};


#endif // _H
