/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SymbolListItem.h"
#include <ListView.h>
#include <stdio.h>

SymbolListItem::SymbolListItem(StockSymbol *symbol)
	:BListItem()
	,fStockSymbol(symbol)
	,fTextOffset(14)
	,fFontSize(14) {
	
	//fCompany->PrintToStream();
}

SymbolListItem::~SymbolListItem() {
	delete fStockSymbol;
}	

StockSymbol*
SymbolListItem::CurrentStockSymbol() {
	return fStockSymbol;
}

void 
SymbolListItem::DrawItem(BView *view, BRect rect, bool complete) {
	
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	rgb_color backgroundColor = ui_color(B_LIST_BACKGROUND_COLOR);
	
	if (IsSelected()) {
		parent->SetHighColor(ui_color(B_LIST_SELECTED_BACKGROUND_COLOR));
	} else if (index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.02));
	}
	parent->FillRect(frame);
	parent->SetHighColor(ui_color( IsSelected() ? B_LIST_SELECTED_ITEM_TEXT_COLOR : B_LIST_ITEM_TEXT_COLOR));
	parent->SetDrawingMode(B_OP_OVER);
	
	BFont font(be_bold_font);
	font.SetSize(fFontSize);
	parent->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);
	float centerY = fh.ascent * 2;
	
	parent->MovePenTo( fTextOffset, frame.LeftBottom().y - (centerY + 2));
	const char *symbol = fStockSymbol->symbol.String();
	parent->DrawString( symbol ); 
	
	font = BFont(be_plain_font);
	font.SetSize(fFontSize);
	parent->SetFont(&font);

	parent->MovePenTo( fTextOffset, frame.LeftBottom().y - (centerY - 1) / 2);
	parent->DrawString( fStockSymbol->name.String() ); 
}

void
SymbolListItem::Update(BView *view, const BFont *font) {
	
	font_height fh;
	font->GetHeight(&fh);
	const float height = fh.ascent + fh.descent + fh.leading + 2;
	
	SetHeight(48);
}
