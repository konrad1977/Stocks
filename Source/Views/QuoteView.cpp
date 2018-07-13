/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "QuoteView.h"
#include "Quote.h"
#include <StringView.h>
#include <string>
#include <sstream>
#include <iostream>

#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>
#include <Box.h>

QuoteView::QuoteView()
	:BView("QuoteView", B_WILL_DRAW)
	,fQuote(NULL)
	,fTitle(NULL)
	,f52High(NULL)
	,f52Low(NULL)
	,fChangePercent(NULL) {
		
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	fTitle = new BStringView("Title", "");
	f52High = new BStringView("52High", "");
	f52Low = new BStringView("52Low", "");
	fChangePercent = new BStringView("ChangePercent", "");
	
	InitLayout();
}

QuoteView::~QuoteView() {
	delete fQuote;
}

const char * 
QuoteView::MakeText(const char * title, double value) {
	BString str;
	str << title << value;
	return str.String();
}

void 
QuoteView::SetChangePercent(float percent) {
	
	std::ostringstream text;

	if (percent < 0 ) {
		fChangePercent->SetHighColor(248,63,58);
		text << percent * 100 << " %";

	} else {
		fChangePercent->SetHighColor(80,195,175);
		text << "+" << percent * 100 << " %";

	}
	BFont font(be_bold_font);
	font.SetSize(14.0);
	//fChangePercent->SetAlignment(B_ALIGN_RIGHT);
	fChangePercent->SetFont(&font);
	fChangePercent->SetText(text.str().c_str());
}

void
QuoteView::SetQuote(Quote *quote) {
	delete fQuote;
	fQuote = quote;
	
	fTitle->SetText(MakeText("Latest price: $",  quote->latestPrice));
	f52High->SetText(MakeText("52 Week high: $",  quote->week52High));
	f52Low->SetText(MakeText("52 Week low: $",  quote->week52Low));
	SetChangePercent(quote->changePercent);
}

BBox *
QuoteView::MakeSeparator() const {
	BBox *separator = new BBox("Separator");
	separator->SetExplicitMinSize(BSize(100, B_SIZE_UNSET));
	separator->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, 1));
	return separator;
}

void
QuoteView::InitLayout() {
	
	BGroupLayout *groupLayout = new BGroupLayout(B_HORIZONTAL);
	SetLayout(groupLayout);
				
	BView *group = BGroupLayoutBuilder(B_VERTICAL, 0)
		.SetInsets(10,10,10,10)
		.Add(fChangePercent)
		.Add(fTitle)
		.Add(MakeSeparator())
		.Add(f52High)
		.Add(f52Low)
		.AddGlue()
		.TopView();
	
	AddChild(group);
}

