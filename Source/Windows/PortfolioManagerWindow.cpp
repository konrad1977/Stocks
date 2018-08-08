/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "PortfolioManagerWindow.h"
#include "PortfolioManager.h"
#include "PortfolioWindow.h"
#include "Portfolio.h"
#include "Constants.h"

#include <Catalog.h>
#include <Autolock.h>
#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

#include <interface/MenuBar.h>

#include <posix/stdio.h>

const uint32 kNewPortfolio	= 'kNPFM';

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PortfolioManagerWindow"

PortfolioManagerWindow::PortfolioManagerWindow(PortfolioManager *manager) 
	:BWindow(BRect(30,30, 300, 400), B_TRANSLATE("PortfolioManager"), B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE)
	,fPortfolioWindow(NULL)
	,fPortfolioManager(manager)
	,fMenuBar(NULL)
{
	InitLayout();
	CenterOnScreen();
}
	
PortfolioManagerWindow::~PortfolioManagerWindow() 
{
	delete fPortfolioWindow;
}

void
PortfolioManagerWindow::InitLayout() 
{
	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	layout->SetSpacing(0);
	SetLayout(layout);
	
	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(B_TRANSLATE("New..."), kNewPortfolio, 'N')
			.AddSeparator()
			.AddItem(B_TRANSLATE("Quit"), B_QUIT_REQUESTED, 'Q')
		.End();
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.AddGlue();
}

void
PortfolioManagerWindow::HandleNewPortfolioMessage(BMessage &message) 
{
	BString portFolioName;
	if (message.FindString("PortfolioName", &portFolioName) == B_OK ) {
		Portfolio *portfolio = new Portfolio(portFolioName);
		if (fPortfolioManager->AddPortfolio(portfolio)) {
//			BAutolock locker(fPortfolioWindow);
			fPortfolioWindow->Quit();		
			
		} else {
			if (fPortfolioWindow == NULL) {
				fPortfolioWindow = new PortfolioWindow(this);
			}
			fPortfolioWindow->SetAlreadyExistingName(portFolioName);
			fPortfolioWindow->Activate();	
			printf("Already exist\n");
		}
	}
}

void
PortfolioManagerWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
		case kNewPortfolio: {
			if (fPortfolioWindow == NULL) {
				fPortfolioWindow = new PortfolioWindow(this);
			}
			fPortfolioWindow->Show();
			break;
		}
		
		case kCreateNewPortfolioMessage: {
			HandleNewPortfolioMessage(*message);
			break;
		}
		default:
		BWindow::MessageReceived(message);
	}
}
