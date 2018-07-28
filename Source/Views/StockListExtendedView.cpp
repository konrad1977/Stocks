/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockListExtendedView.h"
#include "Company.h"
#include "QuoteView.h"
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
	,fCompany(NULL)
	,fMessenger(NULL)
	,fQuoteView(NULL) {
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	fQuoteView = new QuoteView();
	fQuoteView->Hide();

	fDescriptionTextView = new BTextView("TextView");
	fDescriptionTextView->MakeEditable(false);
	fDescriptionTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	rgb_color textColor = ui_color(B_PANEL_TEXT_COLOR);
	fDescriptionTextView->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColor);

	fTitleStringView = new BStringView("Title", "");
	fTitleStringView->SetFont(be_bold_font);
	
	InitLayout();	
}
	
StockListExtendedView::~StockListExtendedView() {
	delete fCompany;
	delete fMessenger;
}

void
StockListExtendedView::AttachedToWindow() {
	if (fQuoteView) 
		fQuoteView->SetTarget(this);
}

void
StockListExtendedView::SetTarget(BHandler *handler) {
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void 
StockListExtendedView::MessageReceieved(BMessage *message) {
	switch (message->what) {
		case kPortfolioButtonPressedMessage: {
			if (fMessenger)
				fMessenger->SendMessage(message);
		}
		break;
	default:
		break;		
	}
}

void
StockListExtendedView::InitLayout() {
	
	BGroupLayout *group = new BGroupLayout(B_VERTICAL);
	SetLayout(group);
	
	BView *leftGroup = BGroupLayoutBuilder(B_VERTICAL, 0)
		.Add(fTitleStringView)
		.Add(fDescriptionTextView)
		.AddGlue()
		.TopView();
		
	BGridLayout *gridLayout = BGridLayoutBuilder(B_HORIZONTAL, 0)
		.SetInsets(10,10,10,10)
		.Add(leftGroup, 0,0 )
		.Add(fQuoteView, 1, 0);
	
	gridLayout->SetMinColumnWidth(0, 500);
	gridLayout->SetMaxColumnWidth(0, 550);
	
	AddChild(gridLayout->View());
}

void
StockListExtendedView::SetCompany(Company *company) {
	delete fCompany;
	fCompany = company;
	fTitleStringView->SetText(company->name.String());
	fDescriptionTextView->SetText(fCompany->description.String());
}

void
StockListExtendedView::SetQuote(Quote *quote, bool hasQuote) {
	
	if (fQuoteView == NULL)  
		return;
		
	if (fQuoteView->IsHidden()) {
		fQuoteView->Show();
	}
	fQuoteView->SetQuote(quote, hasQuote);
}
