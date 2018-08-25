/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockListExtendedView.h"
#include "Company.h"
#include "QuoteView.h"
#include "Constants.h"

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

StockListExtendedView::StockListExtendedView()
	:BView("StockListExtendedView", 0 ) 
	,fDescriptionTextView(NULL)
	,fTitleStringView(NULL)
	,fMessenger(NULL)	
	,fCompany(NULL)
	,fQuoteView(NULL) {
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));	
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
	
	fQuoteView = new QuoteView();
	fQuoteView->Hide();

	fDescriptionTextView = new BTextView("TextView");
	fDescriptionTextView->SetExplicitMinSize(BSize(500, B_SIZE_UNSET));
	
	fDescriptionTextView->MakeEditable(false);
	fDescriptionTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	rgb_color textColor = ui_color(B_PANEL_TEXT_COLOR);
	fDescriptionTextView->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColor);

	fTitleStringView = new BStringView("Title", "");
	fTitleStringView->SetFont(be_bold_font);	

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, B_USE_DEFAULT_SPACING)
		.SetInsets(10,10,10,10)
		.AddGroup(B_VERTICAL, 0)
			.Add(fTitleStringView)
			.Add(fDescriptionTextView)
			.AddGlue()
		.End()
		.AddGroup(B_VERTICAL)
			.Add(fQuoteView)
		.End()
	.End();	
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
