/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockListView.h"
#include "StockRequester.h"
#include "Company.h"

StockListView::StockListView(BRect rect)
	:BListView(rect, "StockListView") {
	
}

StockListView::~StockListView() {
	
}


void
StockListView::MessageReceived(BMessage *message) {

}
