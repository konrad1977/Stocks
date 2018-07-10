/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockListExtendedView.h"
#include "Company.h"

StockListExtendedView::StockListExtendedView(BRect rect)
	:BView(rect, "StockListExtendedView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW)
	,fCompany(NULL) {
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}
	
StockListExtendedView::~StockListExtendedView() {
	delete fCompany;
}

void
StockListExtendedView::SetCompany(Company *company) {
	delete fCompany;
	fCompany = company;
	Invalidate();
}

void
StockListExtendedView::Draw(BRect rect) {
	
	if (fCompany == NULL) {
		return;
	}
	
	BRect r = Bounds();
	r.InsetBy(10,20);
	r.bottom = r.top + 17;
	
	BFont font(be_bold_font);
	font.SetSize(13);
	SetFont(&font);
	SetDrawingMode( B_OP_OVER );
	
	MovePenTo( r.left, r.top);
	const char *name = fCompany->name.String();
	DrawString( name ); 
	
	
	font.SetFace(B_REGULAR_FACE);
	font.SetSize(12);
	SetFont(&font);
	
	r.OffsetBy(0, r.Height());
	MovePenTo( r.left, r.top);
	DrawString( fCompany->exchange.String() ); 
	
	r.OffsetBy(0, r.Height());
	MovePenTo( r.left, r.top);
	DrawString( fCompany->industry.String() ); 
	
	r.OffsetBy(0, r.Height());
	r.bottom += 17;
	MovePenTo( r.left, r.top);
	DrawString( fCompany->description.String() ); 
}
