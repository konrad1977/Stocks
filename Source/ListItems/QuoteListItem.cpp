/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "QuoteListItem.h"
#include <iostream>

#include "Constants.h"
#include "QuoteFormatter.h"

#include <interface/Screen.h>
#include <interface/Window.h>
#include <interface/ListView.h>
#include <locale/Catalog.h>

#include <posix/stdio.h>
#include <posix/string.h>
#include <sstream>
#include <posix/stdlib.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "QuoteListItem"

QuoteListItem::QuoteListItem(Quote quote, bool isReplicant, QuoteType type)
	:BListItem()
	,fQuote(quote)
	,fDrawer(NULL)
	,fQuoteType(type)
	,fIsReplicant(isReplicant)
	,fTransparency(127)
{

}

QuoteListItem::~QuoteListItem()
{
	delete fDrawer;
}

Quote
QuoteListItem::CurrentQuoteItem()
{
	return fQuote;
}

void
QuoteListItem::SetQuote(Quote quote) {
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
	font.SetSize(14);

	std::ostringstream percentText;
	percentText << fQuote.changePercent * 100 << "%";

	const char *percent = percentText.str().c_str();
	rgb_color color;
	if (fQuote.changePercent < 0)  {
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
	font.SetSize(14);

	rgb_color titleColor = fDrawer->TextColor(IsSelected());

	DrawItemSettings settings = { frame, &font, &titleColor, align };
	fDrawer->DrawString(fQuote.symbol.String() , settings);
}

void
QuoteListItem::DrawChangeDollar(BRect frame, alignment align)
{

	BFont font(be_plain_font);
	font.SetSize(13);

	rgb_color titleColor = fDrawer->TextColor(IsSelected());

	QuoteFormatter formatter(&fQuote);
	DrawItemSettings settings = { frame, &font, &titleColor, align };
	DrawText(formatter.ChangeDollar(), settings);
}

void
QuoteListItem::DrawCompanyName(BRect frame, alignment align)
{

	BFont font(be_plain_font);
	font.SetSize(13);

	rgb_color titleColor = fDrawer->TextColor(IsSelected());

	DrawItemSettings settings = { frame, &font, &titleColor, align };
	DrawText(fQuote.companyName.String() , settings);
}

void
QuoteListItem::DrawMarket(BRect frame, alignment align)
{

	BFont font(be_plain_font);
	font.SetSize(13);

	DrawItemSettings settings = { frame, &font, NULL, align };
	fDrawer->DrawString(fQuote.primaryExchange.String(), settings);
}

void
QuoteListItem::DrawLatestPrice(BRect frame, alignment align)
{
	BFont font(be_bold_font);
	font.SetSize(13);

	QuoteFormatter formatter(&fQuote);
	DrawItemSettings settings = { frame, &font, NULL, align };
	DrawText(formatter.LatestPrice(), settings);
}

void
QuoteListItem::DrawBackground(BListView *parent, BRect frame, ListItemDrawer *drawer)
{
	if (fIsReplicant == true && drawer->Transparency() == 0) {
		return;
	}

	const int32 index = parent->IndexOf(this);
	rgb_color backgroundColor = drawer->BackgroundColor(IsSelected());

	if (IsSelected()) {
		backgroundColor = ui_color(B_LIST_SELECTED_BACKGROUND_COLOR);
	} else if (index % 2 == 0) {
		backgroundColor = tint_color(backgroundColor, 1.02);
	}

	parent->SetHighColor(backgroundColor);

	if (fIsReplicant) {
		parent->SetDrawingMode(B_OP_ALPHA);
		parent->FillRoundRect(frame.InsetBySelf(0,2), 3, 3);
	} else {
		parent->SetDrawingMode(B_OP_COPY);
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
	}

	parent->FrameResized(frame.Width(), frame.Height());
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
	QuoteFormatter formatter(&fQuote);
	BRect rect = frame.InsetBySelf(0,8);
	rect.bottom = frame.top + frame.Height() / 7.0;

	rgb_color titleColor 	= fDrawer->TitleColor();
	rgb_color textColor 	= fDrawer->TextColor(IsSelected());
	rgb_color changeColor	= formatter.ChangeColor();

	BFont font(be_bold_font);
	font.SetSize(13);

	//Row 1
	DrawItemSettings priceSettings = { rect, &font, &textColor, B_ALIGN_LEFT };
	DrawText(fQuote.symbol.String(), priceSettings);
	priceSettings.align = B_ALIGN_RIGHT;
	DrawText(formatter.LatestPrice(), priceSettings);
	rect.OffsetBySelf(0, fDrawer->Height(priceSettings));

	//Row 2
	font = be_plain_font;
	font.SetSize(11);
	DrawItemSettings companySettings = { rect, &font, &textColor, B_ALIGN_LEFT };
	DrawText(fQuote.companyName.String(), companySettings);

	DrawItemSettings changeSettings = { rect, &font, &changeColor, B_ALIGN_RIGHT };
	DrawText(formatter.ChangeString(), changeSettings);
	rect.OffsetBySelf(0, fDrawer->Height(changeSettings) * 1.5);

	//Row 3
	DrawItemSettings dailyTitleSettings = { rect, &font, &titleColor, B_ALIGN_LEFT };
	DrawText(B_TRANSLATE("Open"), dailyTitleSettings);

	dailyTitleSettings.align = B_ALIGN_CENTER;
	DrawText(B_TRANSLATE("High"), dailyTitleSettings);

	dailyTitleSettings.align = B_ALIGN_RIGHT;
	DrawText(B_TRANSLATE("Low"), dailyTitleSettings);

	//Row 3 data
	rect.OffsetBy(0, fDrawer->Height(dailyTitleSettings));
	font = be_bold_font;
	font.SetSize(12);


	DrawItemSettings dailyDataSettings = { rect, &font, &textColor, B_ALIGN_LEFT };
	DrawText(formatter.ToString(fQuote.open), dailyDataSettings);

	dailyDataSettings.align = B_ALIGN_CENTER;
	DrawText(formatter.ToString(fQuote.high), dailyDataSettings);

	dailyDataSettings.align = B_ALIGN_RIGHT;
	DrawText(formatter.ToString(fQuote.low), dailyDataSettings);

	rect.OffsetBySelf(0, fDrawer->Height(dailyDataSettings) * 1.5);

	//row4
	font = be_plain_font;
	font.SetSize(11);
	DrawItemSettings volumeTitleSettings = { rect, &font, &titleColor, B_ALIGN_LEFT };
	DrawText(B_TRANSLATE("Avg. volume"), volumeTitleSettings);

	volumeTitleSettings.align = B_ALIGN_CENTER;
	DrawText(B_TRANSLATE("52w. high"), volumeTitleSettings);

	volumeTitleSettings.align = B_ALIGN_RIGHT;
	DrawText(B_TRANSLATE("52w. low"), volumeTitleSettings);

	rect.OffsetBy(0, fDrawer->Height(volumeTitleSettings));

	font = be_bold_font;
	font.SetSize(12);
	volumeTitleSettings.color = &textColor;
	volumeTitleSettings.font = &font;
	volumeTitleSettings.frame = rect;
	volumeTitleSettings.align = B_ALIGN_LEFT;

	DrawText(formatter.ToString(int32(fQuote.avgVolume)), volumeTitleSettings);

	volumeTitleSettings.align = B_ALIGN_CENTER;
	DrawText(formatter.ToString(fQuote.week52High), volumeTitleSettings);

	volumeTitleSettings.align = B_ALIGN_RIGHT;
	DrawText(formatter.ToString(fQuote.week52Low), volumeTitleSettings);
}

void
QuoteListItem::SetTransparency(uint8 transparency)
{
	fTransparency = transparency;
}

void
QuoteListItem::Update(BView *view, const BFont *font)
{
	float height = 15;

	switch (fQuoteType) {
		case SMALL_TYPE:
			height += 12;
			break;
		case NORMAL_TYPE:
			height += 50;
			break;
		case LARGE_TYPE:
			height += 120;
			break;
	}
	SetHeight(height);
}
