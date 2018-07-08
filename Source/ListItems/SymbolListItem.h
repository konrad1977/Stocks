/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _H
#define _H

#include <SupportDefs.h>
#include <ListItem.h>
#include "StockSymbol.h"

class SymbolListItem : public BListItem {
public:
	SymbolListItem(StockSymbol *symbol);
	~SymbolListItem();
	
	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);
								
private:
	StockSymbol *fStockSymbol;
	float fTextOffset;
	float fFontSize;
};

#endif
