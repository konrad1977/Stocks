/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockListExtendedView.h"
#include "Company.h"
#include "Quote.h"
#include "MessageConstants.h"

#include <TextView.h>
#include <StringView.h>
#include <Button.h>
#include <Messenger.h>
#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

#include <stdio.h>

StockListExtendedView::StockListExtendedView(BRect rect)
	:BView(rect, "StockListExtendedView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, 0 ) 
	,fDescriptionTextView(NULL)
	,fTitleStringView(NULL)
	,fAddSymbolButton(NULL)
	,fCompany(NULL)
	,fQuote(NULL)
	,fMessenger(NULL) {
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	fDescriptionTextView = new BTextView("TextView");
	fDescriptionTextView->MakeEditable(false);
	fDescriptionTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	fTitleStringView = new BStringView("Title", "");
	BFont font(be_bold_font);
	fTitleStringView->SetFont(&font);
	fTitleStringView->SetViewColor(255,0,0);

	InitLayout();	
}
	
StockListExtendedView::~StockListExtendedView() {
	delete fCompany;
	delete fQuote;
	delete fMessenger;
}

void
StockListExtendedView::InitLayout() {
	
	BGroupLayout *group = new BGroupLayout(B_VERTICAL);
	SetLayout(group);
				
	BView *redView = new BView("Red", 0);
	redView->SetViewColor(255,0,0);
	
	BView *leftGroup = BGroupLayoutBuilder(B_VERTICAL, 0)
		.SetInsets(10,10,10,10)
		.Add(fTitleStringView)
		.Add(fDescriptionTextView)
		.AddGlue()
		.TopView();
	
	BGridLayout *grid = BGridLayoutBuilder(25)
		.Add(redView, 0, 0);		
	grid->SetMaxColumnWidth(0, 120);
	
	BView *groupView = BGroupLayoutBuilder(B_HORIZONTAL, 0)
		.Add(leftGroup)
		.Add(grid->View())
		.TopView();
	
	AddChild(groupView);
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
	fTitleStringView->SetText(company->name.String());
	fDescriptionTextView->SetText(fCompany->description.String());
	Invalidate();
}

void
StockListExtendedView::SetQuote(Quote *quote) {
	delete fQuote;
	fQuote = quote;
	printf("Quote %s\n", fQuote->symbol.String());
}
