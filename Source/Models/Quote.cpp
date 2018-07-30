/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Quote.h"
#include <stdio.h>

Quote::Quote(BMessage message) {

	message.FindString("companyName", &companyName);
	message.FindString("symbol", &symbol);
	message.FindString("primaryExchange", &primaryExchange);
	message.FindString("sector", &sector);
	
	message.FindDouble("change", &change);
	message.FindDouble("changePercent", &changePercent);
	
	message.FindDouble("high", &high);
	message.FindDouble("low", &low);
	
	message.FindDouble("open", &open);
	message.FindDouble("close", &close);

	message.FindInt32("iexVolume", &volume);
	message.FindInt32("avgTotalVolume", &avgVolume);	
	
	message.FindDouble("latestPrice", &latestPrice);
	message.FindDouble("week52High", &week52High);
	message.FindDouble("week52Low", &week52Low);
}

Quote::~Quote() {

}

void
Quote::PrintToStream() {
	printf("Company: %s\n", companyName.String());
	printf("symbol: %s\n", symbol.String());
	printf("primaryExchange: %s\n", primaryExchange.String());
	printf("%s\n", "-- ");
}
