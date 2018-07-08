/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H


#include <SupportDefs.h>
#include <View.h>

class BTextControl;
class BButton;
class SearchView: public BView {
public:
	SearchView(BRect rect);
	~SearchView();
private:
	
	void _LayoutChildren();

	BTextControl *fSearchTextControl;
	BButton *fSearchButton;		
};


#endif // _H
