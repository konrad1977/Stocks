/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SearchView.h"
#include <TextControl.h>
#include <Button.h>
#include "MessageConstants.h"
#include <LayoutBuilder.h>
#include <GridView.h>

SearchView::SearchView(BRect rect)
	:BView(rect, "SearchView", B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW) {
		
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	_LayoutChildren();
}

SearchView::~SearchView() {
	
}

void
SearchView::_LayoutChildren() {
	
	BGridView *gridView = new BGridView();
	fSearchTextControl = new BTextControl("Search", "Search", "Type here", new BMessage(kSearchButtonPressedMessage));
	
	BLayoutBuilder::Grid<>(gridView)
		.SetInsets(10,10,10,10)
		.Add(fSearchTextControl,0,0);
	
	AddChild(gridView);
	
	BLayoutBuilder::Group<>(this)
		.AddGlue();
}
