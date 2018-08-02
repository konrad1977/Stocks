/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SettingsWindow.h"
#include <Catalog.h>
#include <Messenger.h>

#include "Constants.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

SettingsWindow::SettingsWindow() 
	:BWindow(BRect(0,0, 480, 320), B_TRANSLATE("Settings"), B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_CLOSE_ON_ESCAPE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS )
	,fMessenger(NULL)
{
	CenterOnScreen();
}

SettingsWindow::~SettingsWindow() {
	delete fMessenger;
}

void 
SettingsWindow::SetTarget(BHandler *handler) {
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

bool
SettingsWindow::QuitRequested() {
	BMessage* message = new BMessage(kQuitSettingsWindowMessage);
	fMessenger->SendMessage(message);
	return true;
}

void
SettingsWindow::MessageReceived(BMessage *message) {
	message->PrintToStream();
	switch (message->what) {
		
		default:
			BWindow::MessageReceived(message);
	}
}