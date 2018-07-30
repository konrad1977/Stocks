/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "QuoteFormatter.h"
#include "Quote.h"

#include <String.h>

QuoteFormatter::QuoteFormatter(Quote *quote) 
	:fQuote(quote) {
	
}

QuoteFormatter::~QuoteFormatter() {
	
}

rgb_color 
QuoteFormatter::ChangeColor() {
	if (fQuote->changePercent < 0)  {
		return { 255,64,80 };
	} else {
		return { 102,191,255 };
	}
}

const char *
QuoteFormatter::LatestPrice() const {
	BString str;
	str << "$ " << fQuote->latestPrice;
	return str.String();
}

const char *
QuoteFormatter::ChangeString() const {
	BString str;
	str << "$ " << fQuote->change << " (" << fQuote->changePercent << "%)";
	return str.String();
}
