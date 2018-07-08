/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "StockSymbol.h"
#include <stdio.h>

StockSymbol::StockSymbol(BMessage message) {
	message.FindString("name", &name);
	message.FindString("symbol", &symbol);
}

StockSymbol::~StockSymbol() {

}

void
StockSymbol::PrintToStream() {
	printf("Name: %s\n", name.String());
	printf("Symbol: %s\n", symbol.String());
	printf("%s\n", "-- ");
}
