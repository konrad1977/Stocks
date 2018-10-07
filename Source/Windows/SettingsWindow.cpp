/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SettingsWindow.h"
#include "Portfolio.h"

#include <Catalog.h>
#include <Messenger.h>
#include <Box.h>
#include <Slider.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>

#include "Constants.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

SettingsWindow::SettingsWindow(Portfolio *portfolio)
	:BWindow(BRect(0,0, 480, 320), B_TRANSLATE("Settings"), B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_CLOSE_ON_ESCAPE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS )
	,fMessenger(NULL)
	,fTransparencySlider(NULL)
	,fRefreshRateSlider(NULL)
	,fPortfolio(portfolio)
{
	InitLayout();
	InitSavedValues();
	CenterOnScreen();
}

SettingsWindow::~SettingsWindow()
{
	delete fMessenger;
}

void
SettingsWindow::ShowWithPortfolioName(const char *name)
{
	BString str;
	str << name << " " << B_TRANSLATE("Settings");
	SetTitle(str.String());
	Show();
}

void
SettingsWindow::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

bool
SettingsWindow::QuitRequested()
{
	BMessage message(kQuitSettingsWindowMessage);
	if (fMessenger && fMessenger->IsValid()) {
		fMessenger->SendMessage(&message);
	}
	return true;
}

void
SettingsWindow::InitSavedValues()
{
	const uint8 transparency = fPortfolio->Transparency();
	const uint8 refreshRate = fPortfolio->RefreshRate();

	fTransparencySlider->SetValue(static_cast<int32>(transparency));
	fRefreshRateSlider->SetValue(static_cast<int32>(refreshRate));
	UpdateTransparencyLabel(transparency);
	UpdateRefrehLabel(refreshRate);
}

void
SettingsWindow::UpdateTransparencyLabel(uint8 value)
{
	BString str = fTransparencyLabel;
	str << " " << value;
	fTransparencySlider->SetLabel(str.String());
}

void
SettingsWindow::UpdateRefrehLabel(uint8 value)
{
	BString str = fRefreshLabel;
	str << " " << value;
	fRefreshRateSlider->SetLabel(str.String());
}

void
SettingsWindow::InitLayout()
{
	BGroupLayout *groupLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(groupLayout);

	fTransparencyLabel = B_TRANSLATE("Replicant transparency:");

	fTransparencySlider = new BSlider("Transparency", fTransparencyLabel.String(), new BMessage(kTransparencyChangedMessage), 0, 255, B_HORIZONTAL);
	fTransparencySlider->SetModificationMessage(new BMessage(kTransparenyModificationMessage));

	fRefreshLabel = B_TRANSLATE("Refresh interval (in seconds): ");

	fRefreshRateSlider = new BSlider("Refresh", fRefreshLabel.String(), new BMessage(kRefreshChangedMessage), 1, 300, B_HORIZONTAL);
	fRefreshRateSlider->SetModificationMessage(new BMessage(kRefreshModificationMessage));

	BGroupLayout *transparencyGroup = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(15,10)
		.Add(fTransparencySlider)
		.Add(fRefreshRateSlider);


	BGroupLayout *settingsGroup = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(5,10)
		.Add(transparencyGroup->View());

	AddChild(settingsGroup->View());
}

void
SettingsWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {

		case kTransparencyChangedMessage: {
			uint8 newValue = static_cast<uint8>(fTransparencySlider->Value());
			fPortfolio->SetTransparency(newValue);
			break;
		}

		case kTransparenyModificationMessage: {
			uint8 newValue = static_cast<uint8>(fTransparencySlider->Value());
			UpdateTransparencyLabel(newValue);
			break;
		}

		case kRefreshChangedMessage: {
			uint8 newValue = static_cast<uint8>(fRefreshRateSlider->Value());
			fPortfolio->SetRefreshRate(newValue);
			break;
		}

		case kRefreshModificationMessage: {
			uint8 newValue = static_cast<uint8>(fRefreshRateSlider->Value());
			UpdateRefrehLabel(newValue);
			break;
		}

		default:
			BWindow::MessageReceived(message);
	}
}