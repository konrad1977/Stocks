/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef STOCKLISTEXTENDEDVIEW_H
#define STOCKLISTEXTENDEDVIEW_H


#include <SupportDefs.h>
#include <View.h>

class Company;
class Quote;
class QuoteView;
class BMessenger;
class BHandler;
class BButton;
class BStringView;
class BTextView;
class StockListExtendedView : public BView {
public:
		StockListExtendedView(BRect rect);
		~StockListExtendedView();
		
			void SetCompany(Company *company);
			void SetQuote(Quote *quote, bool hasQuote);
			void SetTarget(BHandler *handler);
			
	virtual void AttachedToWindow();
	virtual void MessageReceieved(BMessage *message);
		
private:
			void InitLayout();

	BTextView 	*fDescriptionTextView;
	BStringView *fTitleStringView;	
	BMessenger 	*fMessenger;
			
	Company 	*fCompany;
	QuoteView 	*fQuoteView;
};

#endif // _H
