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

#include "Constants.h"
#include "QuoteFormatter.h"

#include <posix/stdlib.h>

QuoteListItem::QuoteListItem(Quote *quote, bool isReplicant, QuoteType type)
	:BListItem()
	,fTitle(NULL)
	,fQuote(quote) 
	,fDrawer(NULL)	
	,fQuoteType(type)
	,fIsReplicant(isReplicant) 
	,fTransparency(127)
{	

}

QuoteListItem::QuoteListItem(const char *title)
	:BListItem()
	,fTitle(NULL)
	,fQuote(NULL) 
	,fDrawer(NULL)	
	,fQuoteType(TITLE_TYPE)
	,fIsReplicant(true) 
	,fTransparency(127)
{	
	fTitle = strdup(title);
}

QuoteListItem::~QuoteListItem() 
{
	free(fTitle);
	delete fQuote;
	delete fDrawer;
}	

Quote*
QuoteListItem::CurrentQuoteItem() 
{
	return fQuote;
}

void
QuoteListItem::SetQuote(Quote *quote) {
	delete fQuote;
	fQuote = quote;
}

void 
QuoteListItem::SetQuoteItemType(QuoteType type) 
{
	fQuoteType = type;
}

void
QuoteListItem::DrawText(const char *text, DrawItemSettings settings) 
{
	fDrawer->DrawString(text , settings);
}

void 
QuoteListItem::DrawChangePercent(BRect frame, alignment align) 
{		
	BFont font(be_bold_font);
	font.SetSize(15);

	std::ostringstream percentText;
	percentText << fQuote->changePercent * 100 << "%";

	const char *percent = percentText.str().c_str();
	rgb_color color;
	if (fQuote->changePercent < 0)  {
		color = stockMinusColor;
	} else {
		color = stockPlusColor;
	}
	
	DrawItemSettings settings = { frame, &font, &color, align };
	DrawText(percent , settings);	
}

void 
QuoteListItem::DrawSymbol(BRect frame, alignment align) 
{
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

	QuoteFormatter formatter(fQuote);
	DrawItemSettings settings = { frame, &font, NULL, align };
	DrawText(formatter.ChangeDollar(), settings);
}

void
QuoteListItem::DrawCompanyName(BRect frame, alignment align) {

	BFont font(be_plain_font);
	font.SetSize(13);	
	
	DrawItemSettings settings = { frame, &font, NULL, align }; 
	DrawText(fQuote->companyName.String() , settings);	
}

void 
QuoteListItem::DrawMarket(BRect frame, alignment align) {

	BFont font(be_plain_font);
	font.SetSize(13);		

	DrawItemSettings settings = { frame, &font, NULL, align };
	fDrawer->DrawString(fQuote->primaryExchange.String(), settings);
}

void 
QuoteListItem::DrawLatestPrice(BRect frame, alignment align) 
{	
	BFont font(be_bold_font);
	font.SetSize(15);

	QuoteFormatter formatter(fQuote);
	DrawItemSettings settings = { frame, &font, NULL, align };
	DrawText(formatter.LatestPrice(), settings);
}

void
QuoteListItem::DrawBackground(BListView *parent, BRect frame, ListItemDrawer *drawer)
{
	if (drawer->Transparency() == 0) {
		return;
	}
	
	const int32 index = parent->IndexOf(this);

	rgb_color backgroundColor = drawer->BackgroundColor(IsSelected());

	if (fIsReplicant) {
		parent->SetHighColor(backgroundColor);
	} else if (IsSelected()) {
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
	parent->SetLowColor(backgroundColor);	
}

void 
QuoteListItem::DrawItem(BView *view, BRect rect, bool complete) 
{		
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	if (fDrawer == NULL) {
		fDrawer = new ListItemDrawer(parent, fIsReplicant);
		fDrawer->SetTransparency(fTransparency);
		fDrawer->SetInsets(BSize(10,0));
	}
	
	DrawBackground(parent, rect, fDrawer);
	parent->SetDrawingMode(B_OP_OVER);

	switch (fQuoteType) {
		case SMALL_TYPE: {
			fDrawer->SetInsets(BSize(5,0));
			DrawSmallItem(frame);
			break;
		}
		case NORMAL_TYPE: {
			DrawNormalItem(frame);
			break;
		}
		case LARGE_TYPE: {
			DrawLargeItem(frame);
			break;
		}
		case TITLE_TYPE: {
			DrawTitleItem(frame);
			break;
		}
	}
	parent->FrameResized(rect.Width(), rect.Height());	
}

void
QuoteListItem::DrawTitleItem(BRect frame) 
{
	BFont font(be_bold_font);
	font.SetSize(17);

	DrawItemSettings settings = { frame, &font, NULL, B_ALIGN_CENTER };
	DrawText(fTitle, settings);
}

void
QuoteListItem::DrawSmallItem(BRect frame) 
{
	BRect rect = frame.InsetBySelf(0,10);
	DrawCompanyName(rect);
	DrawChangePercent(rect);
}

void 
QuoteListItem::DrawNormalItem(BRect frame)
{
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
QuoteListItem::DrawLargeItem( BRect frame) 
{	
	QuoteFormatter formatter(fQuote);
	BRect rect = frame.InsetBySelf(0,8);
	rect.bottom = frame.top + frame.Height() / 7.0;
	
	BFont font(be_bold_font);
	font.SetSize(13);
		
	//Row 1
	DrawItemSettings priceSettings = { rect, &font, NULL, B_ALIGN_LEFT };
	DrawText(fQuote->symbol.String(), priceSettings);
	priceSettings.align = B_ALIGN_RIGHT;
	DrawText(formatter.LatestPrice(), priceSettings);
	rect.OffsetBySelf(0, fDrawer->Height(priceSettings));
	
	//Row 2
	font = be_plain_font;
	font.SetSize(11);
	DrawItemSettings companySettings = { rect, &font, NULL, B_ALIGN_LEFT };
	DrawText(fQuote->companyName.String(), companySettings);

	rgb_color changeColor = formatter.ChangeColor();
	DrawItemSettings changeSettings = { rect, &font, &changeColor, B_ALIGN_RIGHT };
	DrawText(formatter.ChangeString(), changeSettings);
	rect.OffsetBySelf(0, fDrawer->Height(changeSettings) * 1.5);
	
	rgb_color titleColor = fDrawer->TitleColor();
	//Row 3
	DrawItemSettings textSettings = { rect, &font, &titleColor, B_ALIGN_LEFT };
	
	DrawText("Open", textSettings);
	
	textSettings.align = B_ALIGN_CENTER;	
	DrawText("High", textSettings);
	
	textSettings.align = B_ALIGN_RIGHT;
	DrawText("Low", textSettings);
	
	rect.OffsetBy(0, fDrawer->Height(textSettings));	
	font = be_bold_font;
	font.SetSize(12);

	textSettings.font = &font;
	textSettings.frame = rect;
	DrawText(formatter.ToString(fQuote->open), textSettings);

	textSettings.align = B_ALIGN_CENTER;
	DrawText(formatter.ToString(fQuote->high), textSettings);
	
	textSettings.align = B_ALIGN_RIGHT;
	DrawText(formatter.ToString(fQuote->low), textSettings);
	
	rect.OffsetBySelf(0, fDrawer->Height(textSettings) * 1.5);
	/*
	//row4
	font = be_plain_font;
	font.SetSize(11);
	settings = { rect, &font, &titleColor, B_ALIGN_LEFT };
	
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
	*/
}

void
QuoteListItem::SetTransparency(uint8 transparency) 
{
	fTransparency = transparency;
}

void
QuoteListItem::Update(BView *view, const BFont *font) 
{	
	font_height fh;
	font->GetHeight(&fh);
	float height = fh.ascent + fh.descent + fh.leading;
	switch (fQuoteType) {
		case SMALL_TYPE:
			height += 14;
			break;
		case NORMAL_TYPE:
			height += 50;
			break;
		case LARGE_TYPE:
			height += 120;
			break;
		case TITLE_TYPE: 
			height += 30;
			break;
	}
	SetHeight(height);
	SetWidth(800);
}
