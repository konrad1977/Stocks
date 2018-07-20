/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "QuoteListItem.h"
#include <ListView.h>
#include <stdio.h>

QuoteListItem::QuoteListItem(Quote *quote)
	:BListItem()
	,fQuote(quote) {	
}

QuoteListItem::~QuoteListItem() {
	delete fQuote;
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
	parent->SetHighColor(40,40,40);
	parent->SetDrawingMode(B_OP_OVER);
	
	BFont font(be_plain_font);
	font.SetSize(17);
	parent->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float center = (frame.Height() - fontHeight) / 2;
	
	parent->MovePenTo( 12, frame.LeftBottom().y - (center + fh.descent));	
	parent->DrawString( fQuote->companyName.String() ); 
}

void
QuoteListItem::Update(BView *view, const BFont *font) {
	
	font_height fh;
	font->GetHeight(&fh);
	const float height = fh.ascent + fh.descent + fh.leading + 20;
	SetHeight(height);
}

