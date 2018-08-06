/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SettingsWindow.h"
#include <Catalog.h>
#include <Messenger.h>
#include <Box.h>
#include <Slider.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>

#include "SettingsManager.h"
#include "Constants.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

SettingsWindow::SettingsWindow() 
	:BWindow(BRect(0,0, 480, 320), B_TRANSLATE("Settings"), B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_CLOSE_ON_ESCAPE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS )
	,fMessenger(NULL)
	,fTransparencySlider(NULL)
{
	InitLayout();
	InitSavedValues();
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
SettingsWindow::InitSavedValues() {
	SettingsManager manager;
	fTransparencySlider->SetValue(static_cast<int32>(manager.Transparency()));
}

void 
SettingsWindow::InitLayout() {

	BGroupLayout *groupLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(groupLayout);

	fTransparencySlider = new BSlider("Transparency", "Replicant transparency", new BMessage(kTransparencyChangeMessage), 0, 255, B_HORIZONTAL);
	fTransparencySlider->SetLimitLabels("0", "255");
	
	BGroupLayout *transparencyGroup = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(5,10)
		.Add(fTransparencySlider);
	

	BGroupLayout *settingsGroup = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(5,10)
		.Add(transparencyGroup->View());
		
	AddChild(settingsGroup->View());
}

void
SettingsWindow::MessageReceived(BMessage *message) {
	switch (message->what) {

		case kTransparencyChangeMessage: {
			uint8 newValue = static_cast<uint8>(fTransparencySlider->Value());
			SettingsManager manager;
			manager.SetTransparency(newValue);
			break;
		}
		
		default:
			BWindow::MessageReceived(message);
	}
}