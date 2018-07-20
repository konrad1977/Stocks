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
	}

SymbolListItem::~SymbolListItem() {
	delete fStockSymbol;
}	

StockSymbol*
SymbolListItem::CurrentStockSymbol() {
	return fStockSymbol;
}

void SymbolListItem::DrawName(BView *view, BRect frame) {

	BFont font(be_plain_font);
	font.SetSize(14);
	view->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);

	rgb_color textColor = ui_color( IsSelected() ? B_LIST_SELECTED_ITEM_TEXT_COLOR : B_LIST_ITEM_TEXT_COLOR);
	
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float center = (frame.Height() - fontHeight) / 2;
	
	view->MovePenTo( 12, frame.RightBottom().y - (center + fh.descent));	
	view->SetHighColor(textColor);
	view->DrawString( fStockSymbol->name.String() ); 
}

void SymbolListItem::DrawSymbol(BView *view, BRect frame) {
		
	BFont font(be_bold_font);
	font.SetSize(14);
	view->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);

	rgb_color textColor = ui_color( IsSelected() ? B_LIST_SELECTED_ITEM_TEXT_COLOR : B_LIST_ITEM_TEXT_COLOR);
	
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float center = (frame.Height() - fontHeight) / 2;
	
	view->MovePenTo( 12, frame.RightBottom().y - (center + fh.descent));	
	view->SetHighColor(textColor);
	view->DrawString( fStockSymbol->symbol.String() ); 
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
	parent->SetDrawingMode(B_OP_OVER);

	BRect halfRect = frame.InsetBySelf(0,4);
	halfRect.bottom -= frame.Height() / 2;
	DrawSymbol(parent, halfRect);
		
	halfRect.OffsetBy(0, halfRect.Height());
	DrawName(parent, halfRect);
}

void
SymbolListItem::Update(BView *view, const BFont *font) {
	
	font_height fh;
	font->GetHeight(&fh);
	const float height = fh.ascent + fh.descent + fh.leading + 30;
	
	SetHeight(height);
}
