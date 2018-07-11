/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockListExtendedView.h"
#include "Company.h"

StockListExtendedView::StockListExtendedView(BRect rect)
	:BView(rect, "StockListExtendedView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_FRAME_EVENTS)
	,fCompany(NULL) {
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BRect r = Bounds();
	r.InsetBy(10,10);
	r.OffsetBy(0, 17);
	r.left = 280;
	r.PrintToStream();
	fDescriptionTextView = new BTextView(r, "TextView", BRect(0,0,r.Width(), r.Height()), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	fDescriptionTextView->MakeEditable(false);
	fDescriptionTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(fDescriptionTextView);
}
	
StockListExtendedView::~StockListExtendedView() {
	delete fCompany;
}

void
StockListExtendedView::SetCompany(Company *company) {
	delete fCompany;
	fCompany = company;
	fDescriptionTextView->SetText(fCompany->description.String());
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
}
