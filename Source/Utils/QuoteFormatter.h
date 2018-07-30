/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef QUOTEFORMATTER_H
#define QUOTEFORMATTER_H

#include <SupportDefs.h>
#include <InterfaceDefs.h>

class Quote;
class QuoteFormatter {
public:
	QuoteFormatter(Quote *quote);
	~QuoteFormatter();
	
	rgb_color ChangeColor();

	const char *ToString(float value) const;
	const char *ToString(int32 value) const;
	const char *ToString(double value) const;
	
	
	const char *LatestPrice() const;
	const char *ChangeString() const; 
	
private:
	Quote *fQuote;
};


#endif // _H
