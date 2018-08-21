/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "PortfolioListItem.h"
#include "Portfolio.h"

#include <interface/ListView.h>

PortfolioListItem::PortfolioListItem(Portfolio *portfolio)
	:BListItem()
	,fPortfolio(portfolio)
	,fDrawer(NULL)
{

}

PortfolioListItem::~PortfolioListItem()
{
	delete fDrawer;
}

void
PortfolioListItem::DrawText(const char *text, DrawItemSettings settings)
{
	fDrawer->DrawString(text , settings);
}

void
PortfolioListItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);

	if (fDrawer == NULL) {
		fDrawer = new ListItemDrawer(parent, false);
		fDrawer->SetTransparency(255);
		fDrawer->SetInsets(BSize(10,10));
	}

	rgb_color backgroundColor = fDrawer->BackgroundColor(IsSelected());

	if (IsSelected()) {
		parent->SetHighColor(ui_color(B_LIST_SELECTED_BACKGROUND_COLOR));
	} else if (index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.02));
	}

	parent->SetDrawingMode(B_OP_COPY);
	parent->FillRect(frame);
	parent->SetDrawingMode(B_OP_OVER);

	DrawPortfolioInfo(frame);

	parent->FrameResized(frame.Width(), frame.Height());
}

void
PortfolioListItem::DrawPortfolioInfo(BRect rect)
{
	BRect frame = rect.InsetBySelf(0,5);
	frame.bottom = frame.top + frame.Height() / 2;

	BFont font(be_plain_font);
	font.SetSize(11);
	rgb_color titleColor = fDrawer->TextColor(IsSelected());
	rgb_color textColor = tint_color(titleColor, 0.7);
	DrawItemSettings symbolSettings = { frame, &font, &textColor, B_ALIGN_LEFT };

	BString stocks;
	stocks << "Stocks: " << fPortfolio->CurrentSymbols()->CountItems();
	DrawText(stocks.String(), symbolSettings);

	font = be_bold_font;
	font.SetSize(14);
	frame = frame.OffsetBySelf(0, frame.Height());
	DrawItemSettings nameSettings = { frame, &font, &titleColor, B_ALIGN_LEFT };
	DrawText(fPortfolio->Name().String(), nameSettings);

}

void
PortfolioListItem::Update(BView *view, const BFont *font)
{
	font_height fh;
	font->GetHeight(&fh);
	float height = fh.ascent + fh.descent + fh.leading;
	height += 30;
	SetHeight(height);
}
