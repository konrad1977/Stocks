/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef QUOTEVIEW_H
#define QUOTEVIEW_H


#include <SupportDefs.h>
#include <View.h>

class BStringView;
class BBox;
class Quote;
class QuoteView : public BView {
public:
		QuoteView();
		~QuoteView();
		
		void SetQuote(Quote *quote);							
private:
		const char * MakeText(const char * title, double value);
		void SetChangePercent(float percent);
		BBox *MakeSeparator() const; 
		void InitLayout();
		
	BStringView *fTitle;
	BStringView *f52High;
	BStringView *f52Low;
	BStringView *fChangePercent;
	
	Quote *fQuote;	
};


#endif // _H
