/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef STOCKLISTVIEW_H
#define STOCKLISTVIEW_H


#include <SupportDefs.h>
#include <ListView.h>

class StockRequester;
class StockListView : public BListView {
public:
	StockListView(BRect rect);
	~StockListView();
	
	virtual void MessageReceived(BMessage *message);					
private:
};


#endif // _H
