/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Quote.h"

Quote::Quote(BMessage message) {
	message.FindString("symbol", &symbol);
	message.FindString("primaryExchange", &primaryExchange);
	message.FindDouble("change", &change);
	message.FindDouble("changePercent", &changePercent);
	message.FindDouble("high", &high);
	message.FindDouble("low", &low);
	
	message.FindDouble("latestPrice", &latestPrice);
	message.FindDouble("week52High", &week52High);
	message.FindDouble("week52Low", &week52Low);
}

Quote::~Quote() {

}

