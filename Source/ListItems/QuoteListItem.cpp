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

QuoteListItem::QuoteListItem(Quote *quote, bool isReplicant, QuoteSize quoteSize)
	:BListItem()
	,fIsReplicant(isReplicant)
	,fQuote(quote) 
	,fQuoteSize(quoteSize) {	
}

QuoteListItem::~QuoteListItem() {
	delete fQuote;
}	

Quote*
QuoteListItem::CurrentQuoteItem() {
	return fQuote;
}

rgb_color
QuoteListItem::BackgroundColor() {
	if (fIsReplicant && IsSelected() == false) {
		BScreen screen;		
		rgb_color color = screen.DesktopColor();
		color.alpha = 140;
		return color;
	}
	return ui_color(B_LIST_BACKGROUND_COLOR);
}

rgb_color
QuoteListItem::TextColor() {

	if (fIsReplicant) {
		rgb_color backgroundColor = BackgroundColor();
		if ( backgroundColor.red < 127 || backgroundColor.green < 127 ||  backgroundColor.blue < 127) {
			rgb_color textColor = { 224, 220, 224 };
			return textColor;
		}
	}
	return ui_color( IsSelected() ? B_LIST_SELECTED_ITEM_TEXT_COLOR : B_LIST_ITEM_TEXT_COLOR);
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
	view->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);

	std::ostringstream percentText;
	percentText << fQuote->changePercent * 100 << "%";

	const char *percent = percentText.str().c_str();
	const float width = font.StringWidth(percent);
	
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float center = (frame.Height() - fontHeight) / 2;
	
	view->MovePenTo( frame.RightBottom().x - width - 12, frame.RightBottom().y - (center + fh.descent));	

	if (fQuote->changePercent < 0)  {
		view->SetHighColor(255,64,80);
	} else {
		view->SetHighColor(102,191,255);
	}
	view->DrawString( percent ); 
}

void
QuoteListItem::DrawCompanyName(BView *view, BRect frame) {
		
	BFont font(be_plain_font);
	font.SetSize(14);
	view->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float center = (frame.Height() - fontHeight) / 2;
	
	view->MovePenTo( 12, frame.LeftBottom().y - (center + fh.descent));	
	view->SetHighColor(TextColor());		
	view->DrawString( fQuote->companyName.String() ); 
}

void 
QuoteListItem::DrawChangeDollar(BView *view, BRect frame) {
		
	BFont font(be_plain_font);
	font.SetSize(15);
	view->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);

	std::ostringstream changeStr;
	changeStr << "$" << fQuote->change;

	const char *change = changeStr.str().c_str();
	const float width = font.StringWidth(change);
	
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float center = (frame.Height() - fontHeight) / 2;
	
	view->MovePenTo( frame.RightBottom().x - width - 12, frame.RightBottom().y - (center + fh.descent));	

	view->SetHighColor(TextColor());
	view->DrawString( change ); 
}

void 
QuoteListItem::DrawLatestPrice(BView *view, BRect frame) {
	
	BFont font(be_bold_font);
	font.SetSize(15);
	view->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);

	std::ostringstream dollarStr;
	dollarStr << "$" << fQuote->latestPrice;

	const char *dollar = dollarStr.str().c_str();
	
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float center = (frame.Height() - fontHeight) / 2;
	
	view->MovePenTo( 12, frame.RightBottom().y - (center + fh.descent));	
	view->SetHighColor(TextColor());	
	view->DrawString( dollar ); 
}

void 
QuoteListItem::DrawItem(BView *view, BRect rect, bool complete) {
	
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	rgb_color backgroundColor = BackgroundColor();
	
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
	
	parent->SetDrawingMode(B_OP_OVER);

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
	halfRect.bottom -= frame.Height() / 2;
	
	DrawCompanyName(parent, halfRect);
	DrawChangePercent(parent, halfRect);

	halfRect.OffsetBy(0, halfRect.Height());
	DrawLatestPrice(parent, halfRect);	
	DrawChangeDollar(parent, halfRect);
}

void 
QuoteListItem::DrawLargeItem(BView *parent, BRect frame) {
	BRect halfRect = frame.InsetBySelf(0,5);
	halfRect.bottom -= frame.Height() / 2;
	
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
			height += 12;
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
