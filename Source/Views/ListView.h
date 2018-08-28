/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <SupportDefs.h>
#include <ListView.h>

class ListView : public BListView {
public:
	ListView(BView *targetView, const char *name);
	
	virtual void MouseDown(BPoint point);
	virtual void MouseUp(BPoint point);
	virtual void MouseMoved(BPoint point, uint32 code, const BMessage *dragMessage);
	
private:
	BView *fTargetView;
};


#endif // _H
