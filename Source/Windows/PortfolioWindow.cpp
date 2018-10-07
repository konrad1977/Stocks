/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "PortfolioWindow.h"
#include "Constants.h"

#include <TextControl.h>
#include <Button.h>

#include <Catalog.h>
#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PortfolioWindow"

PortfolioWindow::PortfolioWindow(BHandler *handler, bool rename)
	:BWindow(BRect(40,40, 200, 200), "", B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_AUTO_UPDATE_SIZE_LIMITS)
	,fMessenger(NULL)	
	,fNameControl(NULL)
	,fIsRenaming(rename)
{
	SetTitle(fIsRenaming ? B_TRANSLATE("Rename portfolio") : B_TRANSLATE("Create new portfolio"));
	fMessenger = new BMessenger(handler);
	InitLayout();
	CenterOnScreen();
}

PortfolioWindow::~PortfolioWindow()
{
	delete fMessenger;
}
	
void 
PortfolioWindow::SendMessage(BMessage message)
{
	BString name(fNameControl->Text());
	message.AddString("PortfolioName", name);
	fMessenger->SendMessage(&message);
	QuitRequested();
	Quit();
}

void 
PortfolioWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kCreateNewPortfolioMessage: {
			BMessage message(kCreateNewPortfolioMessage);
			SendMessage(message);
			break;
		}
		
		case kRenamePortfolioMessage: {
			BMessage message(kRenamePortfolioMessage);
			SendMessage(message);
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
}

void 
PortfolioWindow::SetAlreadyExistingName(BString name)
{
	TextControl()->SetText(name);
}

void 
PortfolioWindow::RenamePortfolio(BString currentName)
{
	TextControl()->SetText(currentName);
}

BTextControl *
PortfolioWindow::TextControl() 
{
	if (fNameControl == NULL) {
		fNameControl = new BTextControl("Name", B_TRANSLATE("Portfolio name:"), B_TRANSLATE("Name"), NULL);
	}
	return fNameControl;
}

void 
PortfolioWindow::InitLayout()
{
	BGroupLayout *group = new BGroupLayout(B_VERTICAL);
	SetLayout(group);
	
	BButton *button;
	if (fIsRenaming) {
		button = new BButton("Rename", B_TRANSLATE("Rename"), new BMessage(kRenamePortfolioMessage));
	} else {
		button = new BButton("Create", B_TRANSLATE("Create"), new BMessage(kCreateNewPortfolioMessage));
	}
		
	button->SetTarget(this);
	button->MakeDefault(true);
	
	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		.SetExplicitMinSize(BSize(400, 40))
		.SetInsets(10,10,10,10)
		.Add(TextControl())
		.Add(button);
		
	TextControl()->MakeFocus();
}

bool
PortfolioWindow::QuitRequested()
{
	BMessage message(kPortfolioQuitMessage);
	fMessenger->SendMessage(&message);
	return BWindow::QuitRequested();
}
