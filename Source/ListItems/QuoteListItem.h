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

enum QuoteType {
	SMALL_TYPE = 0,
	NORMAL_TYPE,
	LARGE_TYPE,
};

class BListView;
class ListItemDrawer;
class QuoteListItem : public BListItem {
public:
	QuoteListItem(Quote quote, bool replicant, QuoteType type = NORMAL_TYPE);
	~QuoteListItem();

	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);

			void SetQuoteItemType(QuoteType type);
			void SetQuote(Quote quote);
			void SetTransparency(uint8 transparency);

	Quote  CurrentQuoteItem();

private:
			void DrawBackground(BListView *view, BRect rect, ListItemDrawer *drawer);
			void DrawSmallItem(BRect frame);
			void DrawNormalItem(BRect frame);
			void DrawLargeItem(BRect frame);
			void DrawTitleItem(BRect frame);

			void DrawText(const char *text, DrawItemSettings settings);

			void DrawSymbol(BRect frame, alignment align = B_ALIGN_LEFT);
			void DrawMarket(BRect frame, alignment align = B_ALIGN_LEFT);
			void DrawCompanyName(BRect frame, alignment align = B_ALIGN_LEFT);
			void DrawChangePercent(BRect frame, alignment align = B_ALIGN_RIGHT);
			void DrawChangeDollar(BRect frame, alignment align = B_ALIGN_RIGHT);
			void DrawLatestPrice(BRect frame, alignment align = B_ALIGN_LEFT);

	Quote 			fQuote;
	ListItemDrawer 	*fDrawer;
	QuoteType 		fQuoteType;
	bool 			fIsReplicant;
	uint8 			fTransparency;
};


#endif // _H
