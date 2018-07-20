/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef QUOTE_H
#define QUOTE_H


#include <SupportDefs.h>
#include <Message.h>
#include <String.h>

class Quote {
public:
	Quote(BMessage message);
	~Quote();
	
	BString companyName;
	BString symbol;
	BString primaryExchange;
	
	double change; //In Dollar
	double changePercent;
	
	double high;
	double low;
	
	double latestPrice;
	double week52High;
	double week52Low;
	
	void PrintToStream();
private:

};

#endif // _H
