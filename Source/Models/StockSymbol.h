/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef STOCKSYMBOL_H
#define STOCKSYMBOL_H


#include <SupportDefs.h>
#include <Message.h>
#include <String.h>

class StockSymbol {
public:
	
	StockSymbol(BMessage message);
	~StockSymbol();
	
	void PrintToStream();

	BString name;
	BString symbol;				
private:

};


#endif // _H
