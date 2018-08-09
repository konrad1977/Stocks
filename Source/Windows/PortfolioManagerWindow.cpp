/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "PortfolioManagerWindow.h"
#include "PortfolioManager.h"
#include "PortfolioWindow.h"
#include "Portfolio.h"
#include "Constants.h"
#include "MainWindow.h"

#include <Catalog.h>
#include <Autolock.h>
#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

#include <interface/ListView.h>
#include <interface/MenuBar.h>

#include <posix/stdio.h>

const uint32 kNewPortfolio	= 'kNPFM';

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PortfolioManagerWindow"

PortfolioManagerWindow::PortfolioManagerWindow() 
	:BWindow(BRect(30,30, 300, 400), B_TRANSLATE("PortfolioManager"), B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE)
	,fPortfolioWindow(NULL)
	,fPortfolioManager(NULL)
	,fMenuBar(NULL)
	,fListView(NULL)
{
	fPortfolioManager = new PortfolioManager(this);

	InitLayout();
	CenterOnScreen();
	ReloadPortfolios();
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
	

	fListView = new BListView("ListView", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	fListView->SetInvocationMessage(new BMessage(kPortfolioManagerSelectMessage));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fListView);
}

void
PortfolioManagerWindow::HandleAlreadyExist(BString previousName) 
{
	printf("HandleAlreadyExist(%s)\n", previousName.String());
	
	if (fPortfolioWindow == NULL) {
		fPortfolioWindow = new PortfolioWindow(this);
		fPortfolioWindow->SetAlreadyExistingName(previousName);
		fPortfolioWindow->Show();
	} else {
		fPortfolioWindow->Activate();
	}
}

void
PortfolioManagerWindow::HandleNewPortfolioMessage(BMessage &message) 
{
	BString portFolioName;
	if (message.FindString("PortfolioName", &portFolioName) == B_OK ) {
		Portfolio *portfolio = new Portfolio(portFolioName);
		if (fPortfolioManager->AddPortfolio(portfolio) == false ) {
			fPortfolioWindow = NULL;
			HandleAlreadyExist(portFolioName);
		}
	}
}

void 
PortfolioManagerWindow::ReloadPortfolios()
{
	BList *portfolios = fPortfolioManager->Portfolios();
	if (portfolios == NULL) {
		return;
	}

	fListView->MakeEmpty();
	
	const int32 items = portfolios->CountItems();
	for (int32 i = 0; i<items; i++) {
		Portfolio *portfolio = static_cast<Portfolio*>(portfolios->ItemAtFast(i));
		fListView->AddItem( new BStringItem(portfolio->Name().String()));
	}
}

void 
PortfolioManagerWindow::ShowWindowWithPortfolio(Portfolio *portfolio) {
	if (portfolio == NULL) {
		return;
	}
	
	MainWindow *window = new MainWindow(portfolio);
	window->Show();
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
			
		case kPortfolioQuitMessage: {
			fPortfolioWindow = NULL;
			break;
		}
		
		case kNewPortfolioCreated: {
			ReloadPortfolios();
			printf("New portfolio added\n");
			break;
		}
		
		case kPortfolioManagerSelectMessage: {
			int32 index = 0;
			if (message->FindInt32("index", &index) == B_OK) {
				BList *portfolios = fPortfolioManager->Portfolios();
				Portfolio *portfolio = static_cast<Portfolio*>(portfolios->ItemAtFast(index));
				ShowWindowWithPortfolio(portfolio);
			}
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
