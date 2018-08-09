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
#include "StockRequester.h"
#include "StockSymbolWindow.h"
#include "StockSymbol.h"
#include "SymbolListItem.h"

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

const uint32 kNewPortfolio	= 'kNPM';

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PortfolioManagerWindow"

PortfolioManagerWindow::PortfolioManagerWindow() 
	:BWindow(BRect(30,30, 300, 400), B_TRANSLATE("PortfolioManager"), B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE)
	,fStockRequester(NULL)
	,fStockSymbolWindow(NULL)
	,fPortfolioWindow(NULL)
	,fPortfolioManager(NULL)
	,fMenuBar(NULL)
	,fListView(NULL)
	,fSymbolList(NULL)
	,fShowStockSymbolListWhenDone(false)
	,fStockSymbolsLoaded(false)
{
	fPortfolioManager = new PortfolioManager(this);
	
	DownloadStockSymbols();
	InitLayout();
	ReloadPortfolios();
	CenterOnScreen();
}
	
PortfolioManagerWindow::~PortfolioManagerWindow() 
{
	delete fPortfolioWindow;
	delete fPortfolioManager;
	delete fStockRequester;
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
			.AddItem(B_TRANSLATE("About..."), B_ABOUT_REQUESTED, 'A')
			.AddItem(B_TRANSLATE("Quit"), B_QUIT_REQUESTED, 'Q')
		.End();
	

	fListView = new BListView("ListView", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	fListView->SetInvocationMessage(new BMessage(kPortfolioManagerSelectMessage));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fListView);
}

StockSymbolWindow *
PortfolioManagerWindow::SymbolWindow()
{
	if (fStockSymbolWindow == NULL) {
		fStockSymbolWindow = new StockSymbolWindow();
	}
	return fStockSymbolWindow;
}

void 
PortfolioManagerWindow::DownloadStockSymbols() 
{
	if (fStockRequester == NULL) {
		fStockRequester = new StockRequester(this);
	}
	fStockRequester->DownloadSymbols();
}

void
PortfolioManagerWindow::ShowStockWindow() 
{
	if (SymbolWindow()->IsHidden() || SymbolWindow()->IsMinimized()) {
		SymbolWindow()->SetStockSymbols(fSymbolList);
		SymbolWindow()->Show();				
	}
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
	window->SetTarget(this);
	SymbolWindow()->SetTarget(window);
	window->Show();
}

void 
PortfolioManagerWindow::HandleStockSearchSymbols(BMessage *message) {
	
	BMessage symbolMessage;
	if (message->FindMessage("Symbols", &symbolMessage) == B_OK) {	
		char *name;
		uint32 type;
		int32 count;
				
		if (fSymbolList == NULL) {
			fSymbolList = new BList();
		}
				
		for (int32 i = 0; symbolMessage.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
			BMessage currentMessage;
			if (symbolMessage.FindMessage(name, &currentMessage) == B_OK) {
				StockSymbol *symbol = new StockSymbol(currentMessage);
				SymbolListItem* symbolListItem = new SymbolListItem(symbol);
				fSymbolList->AddItem(symbolListItem);
			}
		}
	}
	
	fStockSymbolsLoaded = true;
	if (fShowStockSymbolListWhenDone) {
		ShowStockWindow();
	}
}

void
PortfolioManagerWindow::MessageReceived(BMessage *message) {
	
	switch (message->what) {
		case kPortfolioManagerSaveMessage: {
			fPortfolioManager->Save();
			break;
		}
		case B_ABOUT_REQUESTED: {
			printf("Todo not implemented\n");
			break;
		}
		case kNewPortfolio: {
			if (fPortfolioWindow == NULL) {
				fPortfolioWindow = new PortfolioWindow(this);
			}
			fPortfolioWindow->Show();
			break;
		}

		case kUpdateSymbolMessage: { 
			HandleStockSearchSymbols(message);
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
		
		case kShowSearchWindowMessage: {
			ShowStockWindow();
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
