/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef QUOTEVIEW_H
#define QUOTEVIEW_H


#include <SupportDefs.h>
#include <Box.h>

class BStringView;
class BBox;
class Quote;
class QuoteView : public BBox {
public:
		QuoteView();
		~QuoteView();
		
		void SetQuote(Quote *quote);							
private:
		BView * MakeTitleGroup(const char *title, BStringView *right);
		const char * MakeText(const char * title, double value);
		void SetChange(float percent, float dollars);
		BBox *MakeSeparator() const; 
		void InitLayout();
		
	BStringView *fTitle;
	BStringView *f52High;
	BStringView *f52Low;
	BStringView *fChangePercent;
	
	Quote *fQuote;	
};


#endif // _H
