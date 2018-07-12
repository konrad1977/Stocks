/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "QuoteView.h"
#include "Quote.h"

QuoteView::QuoteView()
	:BView("QuoteView", B_WILL_DRAW)
	,fQuote(NULL) {
	
	SetViewColor(255,0,0);		
}

QuoteView::~QuoteView() {
	delete fQuote;
}

void
QuoteView::SetQuote(Quote *quote) {
	delete fQuote;
	fQuote = quote;
	Invalidate();
}

