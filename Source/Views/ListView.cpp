/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ListView.h"
#include <posix/stdio.h>

ListView::ListView(BView *targetView, const char *name)
	:BListView(name, B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE)
	,fTargetView(targetView)
{

}

void 
ListView::MouseDown(BPoint point)
{
	if (fTargetView) {
		fTargetView->MouseDown(ConvertToParent(point));
	}
	BListView::MouseDown(point);	
}

void 
ListView::MouseUp(BPoint point)
{
	if (fTargetView) {
		fTargetView->MouseUp(ConvertToParent(point));
	}
	BListView::MouseUp(point);
}

void
ListView::MouseMoved(BPoint point, uint32 code, const BMessage *dragMessage)
{
	if (fTargetView) {
		fTargetView->MouseMoved(ConvertToParent(point), code, dragMessage);
	}
	BListView::MouseMoved(point, code, dragMessage);
}

