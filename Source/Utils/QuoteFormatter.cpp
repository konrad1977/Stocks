/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "QuoteFormatter.h"
#include "Quote.h"
#include "Constants.h"

#include <String.h>

QuoteFormatter::QuoteFormatter(Quote *quote) 
	:fQuote(quote) {
	
}

QuoteFormatter::~QuoteFormatter() {
	
}

rgb_color 
QuoteFormatter::ChangeColor() {
	return fQuote->changePercent < 0 ? stockMinusColor : stockPlusColor;
}

const char *
QuoteFormatter::ToString(double value) const {
	BString str;
	str << value;
	return str.String();
}

const char *
QuoteFormatter::ToString(float value) const {
	BString str;
	str << value;
	return str.String();
}

const char *
QuoteFormatter::ToString(int32 value) const {
	BString str;
	str << value;
	return str.String();
}

const char *
QuoteFormatter::LatestPrice() const {
	BString str;
	str << "$ " << fQuote->latestPrice;
	return str.String();
}

const char *
QuoteFormatter::ChangeDollar() const {
	BString str;
	str << "$ " << fQuote->change;
	return str.String();
}

const char *
QuoteFormatter::ChangeString() const {
	BString str;
	str << ChangeDollar() << " (" << fQuote->changePercent << "%)";
	return str.String();
}
