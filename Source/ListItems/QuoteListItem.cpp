/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "QuoteListItem.h"
#include <ListView.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <Screen.h>
#include <Window.h>
#include "ListItemDrawer.h"

QuoteListItem::QuoteListItem(Quote *quote, bool isReplicant, QuoteSize quoteSize)
	:BListItem()
	,fIsReplicant(isReplicant)
	,fQuote(quote) 
	,fQuoteSize(quoteSize)
	,fDrawer(NULL) {	
}

QuoteListItem::~QuoteListItem() {
	delete fQuote;
	delete fDrawer;
}	

Quote*
QuoteListItem::CurrentQuoteItem() {
	return fQuote;
}

void
QuoteListItem::SetQuote(Quote *quote) {
	delete fQuote;
	fQuote = quote;
}

void 
QuoteListItem::SetQuoteItemSize(QuoteSize size) {
	fQuoteSize = size;
}

void 
QuoteListItem::DrawChangePercent(BView *view, BRect frame) {
		
	BFont font(be_bold_font);
	font.SetSize(15);

	std::ostringstream percentText;
	percentText << fQuote->changePercent * 100 << "%";

	const char *percent = percentText.str().c_str();
	rgb_color color;
	if (fQuote->changePercent < 0)  {
		color = { 255,64,80 };
	} else {
		color = { 102,191,255 };
	}
	
	DrawItemSettings settings = { frame, &font, &color, B_ALIGN_RIGHT };
	fDrawer->DrawString(percent , settings);	
}

void
QuoteListItem::DrawCompanyName(BView *view, BRect frame) {
	BFont font(be_plain_font);
	font.SetSize(14);	
	
	DrawItemSettings settings = { frame, &font }; 
	fDrawer->DrawString(fQuote->companyName.String() , settings);	
}

void 
QuoteListItem::DrawChangeDollar(BView *view, BRect frame) {
		
	BFont font(be_plain_font);
	font.SetSize(15);

	std::ostringstream changeStr;
	changeStr << "$" << fQuote->change;

	const char *change = changeStr.str().c_str();
	
	DrawItemSettings settings = { frame, &font };
	settings.align = B_ALIGN_RIGHT;
	fDrawer->DrawString(change, settings);
}

void 
QuoteListItem::DrawMarket(BView *view, BRect frame) {
	BFont font(be_plain_font);
	font.SetSize(14);		

	DrawItemSettings settings = { frame, &font };
	fDrawer->DrawString(fQuote->primaryExchange.String(), settings);
}

void 
QuoteListItem::DrawLatestPrice(BView *view, BRect frame) {
	
	BFont font(be_bold_font);
	font.SetSize(15);
	
	std::ostringstream dollarStr;
	dollarStr << "$" << fQuote->latestPrice;
	const char *dollar = dollarStr.str().c_str();

	DrawItemSettings settings = { frame, &font };	
	fDrawer->DrawString(dollar, settings);
}

void 
QuoteListItem::DrawItem(BView *view, BRect rect, bool complete) {
		
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	if (fDrawer == NULL) {
		fDrawer = new ListItemDrawer(parent, fIsReplicant);
		fDrawer->SetInsets(BSize(12,0));
	}
	
	rgb_color backgroundColor = fDrawer->BackgroundColor(IsSelected());
	
	if (IsSelected() && fIsReplicant == false) {
		parent->SetHighColor(ui_color(B_LIST_SELECTED_BACKGROUND_COLOR));
	} else if (index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.02));
	}
	parent->SetDrawingMode(fIsReplicant ? B_OP_ALPHA : B_OP_COPY);
	if (fIsReplicant) {
		parent->FillRoundRect(frame.InsetBySelf(0,2), 3, 3);
	} else {
		parent->FillRect(frame);
	}
	
	parent->SetDrawingMode(B_OP_COPY);
	parent->SetLowColor(backgroundColor);

	switch (fQuoteSize) {
		case SMALL:
			DrawSmallItem(parent, frame);
			break;
		case NORMAL:
			DrawNormalItem(parent, frame);
			break;
		case LARGE:
			DrawLargeItem(parent, frame);
			break;
	}
}

void
QuoteListItem::DrawSmallItem(BView *parent, BRect frame) {
	BRect rect = frame.InsetBySelf(0,10);
	DrawCompanyName(parent, rect);
	DrawChangePercent(parent, rect);
}

void 
QuoteListItem::DrawNormalItem(BView *parent, BRect frame) {

	BRect halfRect = frame.InsetBySelf(0,10);
	halfRect.bottom = frame.top + frame.Height() / 2.0;
	
	DrawCompanyName(parent, halfRect);
	DrawChangePercent(parent, halfRect);

	halfRect.OffsetBy(0, halfRect.Height());
	DrawLatestPrice(parent, halfRect);	
	DrawChangeDollar(parent, halfRect);
}

void 
QuoteListItem::DrawLargeItem(BView *parent, BRect frame) {

	BRect halfRect = frame.InsetBySelf(0,5);
	halfRect.bottom = frame.top + frame.Height() / 4;
	
	DrawMarket(parent, halfRect);
	halfRect.OffsetBy(0, halfRect.Height());

	DrawCompanyName(parent, halfRect);
	DrawChangePercent(parent, halfRect);

	halfRect.OffsetBy(0, halfRect.Height());
	DrawLatestPrice(parent, halfRect);	
	DrawChangeDollar(parent, halfRect);
}

void
QuoteListItem::Update(BView *view, const BFont *font) {
	
	font_height fh;
	font->GetHeight(&fh);
	float height = fh.ascent + fh.descent + fh.leading;
	switch (fQuoteSize) {
		case SMALL:
			height += 14;
			break;
		case NORMAL:
			height += 50;
			break;
		case LARGE:
			height += 90;
			break;
	}
	SetHeight(height);
}
