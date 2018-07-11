/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockListExtendedView.h"
#include "Company.h"
#include "MessageConstants.h"

#include <TextView.h>
#include <Button.h>
#include <Messenger.h>
#include <LayoutBuilder.h>
#include <GridView.h>

StockListExtendedView::StockListExtendedView(BRect rect)
	:BView(rect, "StockListExtendedView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW ) 
	,fDescriptionTextView(NULL)
	,fAddSymbolButton(NULL)
	,fCompany(NULL)
	,fMessenger(NULL) {
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	InitLayout();
}
	
StockListExtendedView::~StockListExtendedView() {
	delete fCompany;
	delete fMessenger;
}

BTextView*
StockListExtendedView::DescriptionTextView() {
	if (fDescriptionTextView == NULL) {

	}
	return fDescriptionTextView;	
}

void
StockListExtendedView::InitLayout() {
	
	BRect r = Bounds();
	r.bottom -= 10;
	r.right -= 10;
	r.left = r.right - 100;
	r.top = r.bottom - 36;
	
	fAddSymbolButton = new BButton(r,"AddSymbolButton", "Add Symbol", new BMessage(kAddSymbolButtonPressedMessage), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	AddChild(fAddSymbolButton);
	
	r = Bounds();
	r.InsetBy(10, 0);
	r.top = 30;
	r.bottom -= 10;
	r.right -= 120;
	
	fDescriptionTextView = new BTextView(r, "TextView", BRect(0,0,r.right - 20,r.bottom - 20), B_FOLLOW_LEFT_RIGHT);
	fDescriptionTextView->MakeEditable(false);
	fDescriptionTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(fDescriptionTextView);
}

void 
StockListExtendedView::SetTarget(BHandler *handler) {
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
StockListExtendedView::SetCompany(Company *company) {
	delete fCompany;
	fCompany = company;
	DescriptionTextView()->SetText(fCompany->description.String());
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
	
//	
//	font.SetFace(B_REGULAR_FACE);
//	font.SetSize(12);
//	SetFont(&font);
//	
//	r.OffsetBy(0, r.Height());
//	MovePenTo( r.left, r.top);
//	DrawString( fCompany->exchange.String() ); 
//	
//	r.OffsetBy(0, r.Height());
//	MovePenTo( r.left, r.top);
//	DrawString( fCompany->industry.String() );  
}
