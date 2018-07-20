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

QuoteListItem::QuoteListItem(Quote *quote)
	:BListItem()
	,fQuote(quote) {	
}

QuoteListItem::~QuoteListItem() {
	delete fQuote;
}	

void QuoteListItem::DrawChange(BView *view, BRect frame) {

	view->SetDrawingMode(B_OP_COPY);
	view->SetHighColor(71,66,71);	
	
	BFont font(be_plain_font);
	font.SetSize(17);
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
		view->SetHighColor(248,63,58);
	} else {
		view->SetHighColor(44,104,233);
	}
	view->DrawString( percent ); 
}

void
QuoteListItem::DrawCompanyName(BView *view, BRect frame) {
	
	view->SetDrawingMode(B_OP_COPY);
	view->SetHighColor(71,66,71);	
	
	BFont font(be_plain_font);
	font.SetSize(17);
	view->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float center = (frame.Height() - fontHeight) / 2;
	
	view->MovePenTo( 12, frame.LeftBottom().y - (center + fh.descent));	
	view->DrawString( fQuote->companyName.String() ); 
}

void 
QuoteListItem::DrawItem(BView *view, BRect rect, bool complete) {
	
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	if (index % 2 == 0) {
		parent->SetHighColor(255, 255, 255);
	} else {
		parent->SetHighColor(245, 245, 245);
	}
	parent->FillRect(frame);
	
	DrawCompanyName(parent, frame);
	DrawChange(parent, frame);
}

void
QuoteListItem::Update(BView *view, const BFont *font) {
	
	font_height fh;
	font->GetHeight(&fh);
	const float height = fh.ascent + fh.descent + fh.leading + 80;
	SetHeight(height);
}

