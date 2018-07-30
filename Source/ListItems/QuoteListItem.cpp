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

#include "QuoteFormatter.h"

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
QuoteListItem::DrawText(const char *text, DrawItemSettings settings) {
	fDrawer->DrawString(text , settings);
}

void 
QuoteListItem::DrawChangePercent(BRect frame, alignment align) {
		
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
	
	DrawItemSettings settings = { frame, &font, &color, align };
	fDrawer->DrawString(percent , settings);	
}

void 
QuoteListItem::DrawSymbol(BRect frame, alignment align) {

	BFont font(be_bold_font);
	font.SetSize(15);	
	
	DrawItemSettings settings = { frame, &font }; 
	settings.align = align;
	fDrawer->DrawString(fQuote->symbol.String() , settings);	
}

void 
QuoteListItem::DrawChangeDollar(BRect frame, alignment align) {
		
	BFont font(be_plain_font);
	font.SetSize(13);

	std::ostringstream change;
	change << "$ " << fQuote->change;
	
	const char *str = change.str().c_str();
	
	DrawItemSettings settings = { frame, &font };
	settings.align = align;
	fDrawer->DrawString(str, settings);
}

void
QuoteListItem::DrawCompanyName(BRect frame, alignment align) {

	BFont font(be_plain_font);
	font.SetSize(13);	
	
	DrawItemSettings settings = { frame, &font, NULL, align }; 
	fDrawer->DrawString(fQuote->companyName.String() , settings);	
}

void 
QuoteListItem::DrawMarket(BRect frame, alignment align) {

	BFont font(be_plain_font);
	font.SetSize(13);		

	DrawItemSettings settings = { frame, &font, NULL, align };
	fDrawer->DrawString(fQuote->primaryExchange.String(), settings);
}

void 
QuoteListItem::DrawLatestPrice(BRect frame, alignment align) {
	
	BFont font(be_bold_font);
	font.SetSize(15);
	
	std::ostringstream dollarStr;
	dollarStr << "$" << fQuote->latestPrice;
	const char *dollar = dollarStr.str().c_str();

	DrawItemSettings settings = { frame, &font, NULL, align };
	fDrawer->DrawString(dollar, settings);
}

void 
QuoteListItem::DrawItem(BView *view, BRect rect, bool complete) {
		
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	if (fDrawer == NULL) {
		fDrawer = new ListItemDrawer(parent, fIsReplicant);
		fDrawer->SetInsets(BSize(10,0));
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
		case SMALL: {
			fDrawer->SetInsets(BSize(5,0));
			DrawSmallItem(frame);
			break;
		}
		case NORMAL: {
			DrawNormalItem(frame);
			break;
		}
		case LARGE: {
			DrawLargeItem(frame);
			break;
		}
	}
}

void
QuoteListItem::DrawSmallItem(BRect frame) {
	BRect rect = frame.InsetBySelf(0,10);
	DrawCompanyName(rect);
	DrawChangePercent(rect);
}

void 
QuoteListItem::DrawNormalItem(BRect frame) {

	BRect rect = frame.InsetBySelf(0,5);
	rect.bottom = frame.top + frame.Height() / 3.0;
	
	DrawSymbol(rect);
	DrawChangePercent(rect);

	rect.OffsetBy(0, rect.Height());

	DrawCompanyName(rect);
	DrawChangeDollar(rect);

	rect.OffsetBy(0, rect.Height());
	DrawLatestPrice(rect);	
}

void 
QuoteListItem::DrawLargeItem( BRect frame) {
	
	QuoteFormatter formatter(fQuote);
	BRect rect = frame.InsetBySelf(0,2);
	rect.bottom = frame.top + frame.Height() / 7.0;
	
	BFont font(be_bold_font);
	font.SetSize(13);
		
	//Row 1
	DrawItemSettings settings = { rect, &font, NULL, B_ALIGN_LEFT };
	DrawText(fQuote->symbol.String(), settings);
	settings.align = B_ALIGN_RIGHT;
	DrawText(formatter.LatestPrice(), settings);
	rect.OffsetBySelf(0, fDrawer->Height(settings));
	
	//Row 2
	font = be_plain_font;
	font.SetSize(11);
	settings = { rect, &font, NULL, B_ALIGN_LEFT };
	DrawText(fQuote->companyName.String(), settings);

	rgb_color changeColor = formatter.ChangeColor();
	settings = { rect, &font, &changeColor, B_ALIGN_RIGHT };
	DrawText(formatter.ChangeString(), settings);
	rect.OffsetBySelf(0, fDrawer->Height(settings) * 2);
	
	//Row 3
	settings = { rect, &font, NULL, B_ALIGN_LEFT };
	
	DrawText("Open", settings);
	
	settings.align = B_ALIGN_CENTER;	
	DrawText("High", settings);
	
	settings.align = B_ALIGN_RIGHT;
	DrawText("Low", settings);
	
	rect.OffsetBy(0, fDrawer->Height(settings));	
	font = be_bold_font;
	font.SetSize(12);

	settings = { rect, &font, NULL };	
	DrawText(formatter.ToString(fQuote->open), settings);

	settings.align = B_ALIGN_CENTER;
	DrawText(formatter.ToString(fQuote->high), settings);
	
	settings.align = B_ALIGN_RIGHT;
	DrawText(formatter.ToString(fQuote->low), settings);
	
	rect.OffsetBySelf(0, fDrawer->Height(settings) * 1.5);
	
	//row4
	font = be_plain_font;
	font.SetSize(11);
	settings = { rect, &font, NULL, B_ALIGN_LEFT };
	
	DrawText("Avg. volume", settings);
	
	settings.align = B_ALIGN_CENTER;	
	DrawText("52w. high", settings);
	
	settings.align = B_ALIGN_RIGHT;
	DrawText("52w. low", settings);
	
	rect.OffsetBy(0, fDrawer->Height(settings));	

	font = be_bold_font;
	font.SetSize(12);

	settings = { rect, &font, NULL };	
	DrawText(formatter.ToString(int32(fQuote->avgVolume)), settings);

	settings.align = B_ALIGN_CENTER;
	DrawText(formatter.ToString(fQuote->week52High), settings);
	
	settings.align = B_ALIGN_RIGHT;
	DrawText(formatter.ToString(fQuote->week52Low), settings);
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
			height += 120;
			break;
	}
	SetHeight(height);
}
