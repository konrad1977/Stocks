/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef QUOTELISTITEM_H
#define QUOTELISTITEM_H

#include <SupportDefs.h>
#include <InterfaceDefs.h>
#include <ListItem.h>

#include "ListItemDrawer.h"
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
			void DrawSmallItem(BRect frame);
			void DrawNormalItem(BRect frame);
			void DrawLargeItem( BRect frame);
			
			void DrawText(const char *text, DrawItemSettings settings);
			
			void DrawSymbol(BRect frame, alignment align = B_ALIGN_LEFT);
			void DrawMarket(BRect frame, alignment align = B_ALIGN_LEFT);
			void DrawCompanyName(BRect frame, alignment align = B_ALIGN_LEFT);
			void DrawChangePercent(BRect frame, alignment align = B_ALIGN_RIGHT);
			void DrawChangeDollar(BRect frame, alignment align = B_ALIGN_RIGHT);
			void DrawLatestPrice(BRect frame, alignment align = B_ALIGN_LEFT);
			
	Quote *fQuote;
	QuoteSize fQuoteSize;
	ListItemDrawer *fDrawer;
	bool fIsReplicant;
};


#endif // _H
