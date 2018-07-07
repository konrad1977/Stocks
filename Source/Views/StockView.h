/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef STOCKVIEW_H
#define STOCKVIEW_H


#include <SupportDefs.h>
#include <View.h>

class StockRequester;
class StockView : public BView {
public:
	StockView(BRect rect);
	~StockView();
	
	virtual void MessageReceived(BMessage *message);
	virtual void AttachedToWindow();
								
private:
	StockRequester *fStockRequester;
};


#endif // _H
