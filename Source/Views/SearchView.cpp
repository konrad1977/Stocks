/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SearchView.h"
#include <TextControl.h>
#include <Button.h>
#include <Messenger.h>
#include "MessageConstants.h"
#include <LayoutBuilder.h>
#include <GridView.h>
#include <string.h>

SearchView::SearchView(BRect rect)
	:BView(rect, "SearchView", B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW)
	,fSearchTextControl(NULL)
	,fSearchButton(NULL)
	,fMessenger(NULL) {
		
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

SearchView::~SearchView() {
	
}

void
SearchView::AttachedToWindow() {
	_LayoutChildren();
}

void
SearchView::MessageReceived(BMessage *message) {
	switch (message->what) {
		case kSearchTextChangedMessage: {
			const char *text = TextControl()->Text();		
			BMessage msg(kSearchTextChangedMessage);
			msg.AddString("searchText", text);
			fMessenger->SendMessage(&msg);
		}
		break;
		default:
			break;
	}
}

void
SearchView::SetTarget(BHandler *handler) {
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

BTextControl *
SearchView::TextControl() {
	if (fSearchTextControl == NULL) {
		fSearchTextControl = new BTextControl("Search", "Search", "Type here", NULL);
		fSearchTextControl->SetModificationMessage(new BMessage(kSearchTextChangedMessage));
		fSearchTextControl->SetTarget(this);
	}
	return fSearchTextControl;
}

void
SearchView::_LayoutChildren() {
	
	BGridView *gridView = new BGridView();
	fSearchButton = new BButton("Search", new BMessage(kSearchButtonPressedMessage));
	
	BLayoutBuilder::Grid<>(gridView)
		.SetInsets(10,10,10,10)
		.Add(TextControl(),0,0)
		.Add(fSearchButton,1,0);
	
	AddChild(gridView);
	
	BLayoutBuilder::Group<>(this)
		.AddGlue();
}

