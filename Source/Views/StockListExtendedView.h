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
class BMessenger;
class BHandler;
class BButton;
class BTextView;
class StockListExtendedView : public BView {
public:
		StockListExtendedView(BRect rect);
		~StockListExtendedView();
		
			void SetCompany(Company *company);
			void SetQuote(Quote *quote);
	virtual void Draw(BRect rect);
			void SetTarget(BHandler *handler);
private:
		BTextView *DescriptionTextView();
			void InitLayout();

	BTextView 	*fDescriptionTextView;
	BButton 	*fAddSymbolButton;
			
	Company 	*fCompany;
	Quote		*fQuote;
	BMessenger 	*fMessenger;
};

#endif // _H
