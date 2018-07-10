/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StockListExtendedView.h"


StockListExtendedView::StockListExtendedView(BRect rect)
	:BView(rect, "StockView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW) {
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}
	
StockListExtendedView::~StockListExtendedView() {

}
