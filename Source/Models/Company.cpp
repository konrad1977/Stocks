/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Company.h"
#include <stdio.h>

Company::Company(BMessage message) {
	message.FindString("companyName", &name);
	message.FindString("symbol", &symbol);
	message.FindString("exchange", &exchange);
	message.FindString("industry", &industry);
	message.FindString("website", &website);
	message.FindString("description", &description);
	message.FindString("CEO", &CEO);
	message.FindString("sector", &sector);
}

Company::~Company() {

}

void
Company::PrintToStream() {
	printf("-- Company: %s\n", name.String());
	printf("Symbol: %s\n", symbol.String());
	printf("exchange: %s\n", exchange.String());
	printf("website: %s\n", website.String());
	printf("description: %s\n", description.String());
	printf("CEO: %s\n", CEO.String());
	printf("sector: %s\n", sector.String());
	printf("%s\n", "-- ");
}


