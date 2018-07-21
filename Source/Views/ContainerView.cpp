/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ContainerView.h"

#include <Dragger.h>

ContainerView::ContainerView(BRect rect)
	:BView(rect, "ContainerView", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS) {
		
	SetupViews();
}
	
ContainerView::ContainerView(BMessage *archive) 
	:BView(archive) {
	SetupViews();
}
ContainerView::~ContainerView() {

}

status_t
ContainerView::Archive(BMessage* into, bool deep = true) const {
	status_t status;

	status = BView::Archive(into, deep);
	if (status < B_OK) {	
		return status;
	}

	status = SaveState(into, deep);
	if (status < B_OK) {
		return status;
	}
	return B_OK;
}

static BArchivable*	
ContainerView::Instantiate(BMessage* archive) {
	if (!validate_instantiation(archive, "ContainerView")) {
		return NULL;
	}
	return new ContainerView(archive);
}

status_t	
ContainerView::SaveState(BMessage* into, bool deep = true) const {
	status_t status;
	return B_OK;
}
 
void
ContainerView::SetupViews() {
	
}
