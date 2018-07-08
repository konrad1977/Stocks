/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SymbolListItem.h"
#include <ListView.h>
#include <stdio.h>

SymbolListItem::SymbolListItem(StockSymbol *symbol)
	:BListItem()
	,fStockSymbol(symbol) {
	
	//fCompany->PrintToStream();
}

SymbolListItem::~SymbolListItem() {
	delete fStockSymbol;
}	

void 
SymbolListItem::DrawItem(BView *view, BRect rect, bool complete) {
	
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	if (index % 2 == 0) {
		parent->SetHighColor(255, 255, 255);
	} else {
		parent->SetHighColor(248, 248, 248);
	}
	parent->FillRect(frame);
	parent->SetHighColor(60,60,60);
	parent->SetDrawingMode(B_OP_OVER);
	
	BFont font(be_bold_font);
	font.SetSize(13);
	parent->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);
	float centerY = (frame.Height() - fh.ascent) / 2;
	
	parent->MovePenTo( 12, frame.LeftBottom().y - centerY);
	const char *symbol = fStockSymbol->symbol.String();
	parent->DrawString( symbol ); 
	const float symbolWidth = font.StringWidth(symbol);
	
	font = BFont(be_plain_font);
	font.SetSize(13);
	parent->SetFont(&font);

	parent->MovePenTo( 14 + symbolWidth, frame.LeftBottom().y - centerY);
	parent->SetHighColor(30,30,30);
	parent->DrawString( fStockSymbol->name.String() ); 
}

void
SymbolListItem::Update(BView *view, const BFont *font) {
	
	font_height fh;
	font->GetHeight(&fh);
	const float height = fh.ascent + fh.descent + fh.leading + 2;
	
	SetHeight(21);
}
