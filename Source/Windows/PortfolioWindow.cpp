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

PortfolioWindow::PortfolioWindow(BHandler *handler) 
	:BWindow(BRect(40,40, 200, 200), B_TRANSLATE("New portfolio"), B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_AUTO_UPDATE_SIZE_LIMITS)
	,fMessenger(NULL)	
	,fNameControl(NULL)
{
	fMessenger = new BMessenger(handler);
	InitLayout();
	CenterOnScreen();
}

PortfolioWindow::~PortfolioWindow()
{
	delete fMessenger;
}
	
void 
PortfolioWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kCreateNewPortfolioMessage: {
			BMessage message(kCreateNewPortfolioMessage);
			BString name(fNameControl->Text());
			message.AddString("PortfolioName", name);
			fMessenger->SendMessage(&message);
			QuitRequested();
			Quit();
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
	
	BButton *createButton = new BButton("Create", "Create", new BMessage(kCreateNewPortfolioMessage));
	createButton->SetTarget(this);
	createButton->MakeDefault(true);
	
	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		.SetExplicitMinSize(BSize(400, 40))
		.SetInsets(10,10,10,10)
		.Add(TextControl())
		.Add(createButton);
		
	TextControl()->MakeFocus();
}

bool
PortfolioWindow::QuitRequested()
{
	BMessage message(kPortfolioQuitMessage);
	fMessenger->SendMessage(&message);
	return true;
}
