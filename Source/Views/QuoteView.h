/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef QUOTEVIEW_H
#define QUOTEVIEW_H


#include <SupportDefs.h>
#include <View.h>

class Quote;
class QuoteView : public BView {
public:
		QuoteView();
		~QuoteView();
		
		void SetQuote(Quote *quote);							
private:
	Quote *fQuote;	
};


#endif // _H
