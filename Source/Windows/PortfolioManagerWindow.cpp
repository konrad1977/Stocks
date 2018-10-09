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
#include "PortfolioListItem.h"
#include "ListView.h"

#include <locale/Catalog.h>
#include <Autolock.h>

#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

#include <support/Autolock.h>
#include <app/Application.h>
#include <interface/ListView.h>
#include <interface/MenuBar.h>
#include <interface/ScrollView.h>

#include <posix/stdio.h>

const uint32 kNewPortfolio	= 'kNPM';

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PortfolioManagerWindow"

PortfolioManagerWindow::PortfolioManagerWindow()
	:BWindow(BRect(30,30, 1, 1), B_TRANSLATE("PortfolioManager"), B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE | B_AUTO_UPDATE_SIZE_LIMITS)
	,fStockRequester(NULL)
	,fStockSymbolWindow(NULL)
	,fPortfolioWindow(NULL)
	,fPortfolioManager(NULL)
	,fMenuBar(NULL)
	,fRemoveSelectedItem(NULL)
	,fRenameSelectedItem(NULL)
	,fListView(NULL)
	,fSymbolList(NULL)
	,fShowStockSymbolListWhenDone(false)
	,fStockSymbolsLoaded(false)
	,fCurrentSelectedItemIndex(-1)
{
	fPortfolioManager = new PortfolioManager(this);

	DownloadStockSymbols();
	InitLayout();
	ReloadPortfolios();
	CenterOnScreen();
}

PortfolioManagerWindow::~PortfolioManagerWindow()
{

	if (fPortfolioWindow) {
		fPortfolioWindow->Lock();
		fPortfolioWindow->Quit();
	}

	if (fStockSymbolWindow) {
		fStockSymbolWindow->Lock();
		fStockSymbolWindow->Quit();
	}

	while( fSymbolList->CountItems() ) {
		delete fSymbolList->RemoveItem(int32(0));
	}

	delete fStockRequester;
	delete fPortfolioManager;
	delete fSymbolList;
}

void
PortfolioManagerWindow::InitLayout()
{
	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	layout->SetSpacing(0);
	SetLayout(layout);

	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(B_TRANSLATE("New"B_UTF8_ELLIPSIS), kNewPortfolio, 'N')
			.AddSeparator()
			.AddItem(B_TRANSLATE("About" B_UTF8_ELLIPSIS), B_ABOUT_REQUESTED, 'A')
			.AddItem(B_TRANSLATE("Quit"), B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu(B_TRANSLATE("Edit"))
			.AddItem(fRemoveSelectedItem = new BMenuItem(B_TRANSLATE("Delete" B_UTF8_ELLIPSIS), new BMessage(kRemoveSelectedListItem), 'D'))
			.AddItem(fRenameSelectedItem = new BMenuItem(B_TRANSLATE("Rename" B_UTF8_ELLIPSIS), new BMessage(kRenameSelectedListItem), 'E'))
		.End();


	fListView = new BListView("ListView", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	fListView->SetExplicitMinSize(BSize(400, B_SIZE_UNSET));
	fListView->SetExplicitMaxSize(BSize(800, B_SIZE_UNLIMITED));
	BScrollView *scrollView = new BScrollView("Scrollview", fListView, B_SUPPORTS_LAYOUT, false, true);

	fListView->SetInvocationMessage(new BMessage(kListInvocationMessage));
	fListView->SetSelectionMessage( new BMessage(kListSelectMessage));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(scrollView);

	fRemoveSelectedItem->SetEnabled(false);
	fRenameSelectedItem->SetEnabled(false);
}

StockSymbolWindow *
PortfolioManagerWindow::SymbolWindow()
{
	if (fStockSymbolWindow == NULL) {
		fStockSymbolWindow = new StockSymbolWindow();
		fStockSymbolWindow->SetTarget(this);
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
	if (fPortfolioWindow == NULL) {
		fPortfolioWindow = new PortfolioWindow(this, false);
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
		PortfolioListItem *listItem = new PortfolioListItem(portfolio);
		fListView->AddItem(listItem);
	}
	ResizeWindow(10);
}

void
PortfolioManagerWindow::ResizeWindow(int32 maxItemsToFit)
{
	int32 count = fListView->CountItems();
	count = (count < maxItemsToFit) ? count : maxItemsToFit;
	BRect itemRect = fListView->ItemFrame(0);
	float itemHeight = itemRect.Height() + 4.0f;
	float height = count * itemHeight;

	if (height == 0) {
		height = 40;
	}

	fListView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, height));
	fListView->SetExplicitMinSize(BSize(300, height));
}

BWindow*
PortfolioManagerWindow::PortfolioWindowWithName(BString str)
{
	const int32 items = be_app->CountWindows();

	for (int32 i = 0; i<items; i++) {
		BWindow *activeWindow = be_app->WindowAt(i);
		if (activeWindow && activeWindow->Title() == str) {
			return activeWindow;
		}
	}
	return NULL;
}

void
PortfolioManagerWindow::ShowWindowWithPortfolio(Portfolio *portfolio) {

	if (portfolio == NULL) {
		return;
	}

	MainWindow *window = dynamic_cast<MainWindow*>(PortfolioWindowWithName(portfolio->Name()));
	if (window != NULL) {
		window->Activate();
	} else {
		window = new MainWindow(portfolio);
		window->SetTarget(this);
		window->Show();
	}
	SymbolWindow()->SetSymbolTarget(window);
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
				StockSymbol symbol(currentMessage);
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

Portfolio *
PortfolioManagerWindow::CurrentSelectedPortfolio() const
{
	BList *portfolios = fPortfolioManager->Portfolios();
	if (portfolios == NULL) {
		return NULL;
	}
	return static_cast<Portfolio*>(portfolios->ItemAtFast(fCurrentSelectedItemIndex));
}

void
PortfolioManagerWindow::MessageReceived(BMessage *message) {

	switch (message->what) {
		case kPortfolioUpdatedSettingsMessage:{
			printf("PortfolioManagerWindow :: kPortfolioUpdatedSettingsMessage\n");
			break;
		}
		case kPortfolioManagerSaveMessage: {
			fPortfolioManager->Save();
			break;
		}
		case B_ABOUT_REQUESTED: {
			be_app->PostMessage(message);
			break;
		}
		case kNewPortfolio: {
			if (fPortfolioWindow == nullptr) {
				fPortfolioWindow = new PortfolioWindow(this, false);
			}
			fPortfolioWindow->Show();
			break;
		}

		case kUpdateSymbolMessage: {
			HandleStockSearchSymbols(message);
			break;
		}

		case kPortfolioQuitMessage: {
			fPortfolioWindow = nullptr;
			break;
		}

		case kPortfolioManagerChangeMessage: {
			ReloadPortfolios();
			break;
		}

		case kShowSearchWindowMessage: {
			BString portfolio;
			if (message->FindString("PortfolioName", &portfolio) != B_OK) {
				return;
			}

			if (BWindow *window = PortfolioWindowWithName(portfolio)) {
				SymbolWindow()->SetSymbolTarget(window);
				ShowStockWindow();
			}
			break;
		}

		case kRemoveSelectedListItem: {
			if (Portfolio *portfolio = CurrentSelectedPortfolio()) {
				fPortfolioManager->RemovePortfolio(portfolio);
			}
			break;
		}

		case kRenameSelectedListItem : {
			if (Portfolio *portfolio = CurrentSelectedPortfolio()) {
				fPortfolioWindow = new PortfolioWindow(this, true);
				fPortfolioWindow->RenamePortfolio(portfolio->Name().String());
				fPortfolioWindow->Show();
			}
			break;
		}

		case kHideSearchWindowMessaage: {
			printf("kHideSearchWindowMessaage \n");
			fStockSymbolWindow = NULL;
			break;
		}

		case kListSelectMessage: {
			if (message->FindInt32("index", &fCurrentSelectedItemIndex) == B_OK) {

				fRemoveSelectedItem->SetEnabled(fCurrentSelectedItemIndex != -1);
				fRenameSelectedItem->SetEnabled(fCurrentSelectedItemIndex != -1);

				if (Portfolio *portfolio = CurrentSelectedPortfolio()) {
					BString str;
					str << "Delete '" << portfolio->Name().String() << "'";
					fRemoveSelectedItem->SetLabel(str.String());

					str = "";
					str << "Rename '" << portfolio->Name().String() << "'";
					fRenameSelectedItem->SetLabel(str.String());
				}
			}
			break;
		}

		case kListInvocationMessage: {
			int32 index = 0;
			if (message->FindInt32("index", &index) == B_OK) {
				BList *portfolios = fPortfolioManager->Portfolios();
				Portfolio *portfolio = static_cast<Portfolio*>(portfolios->ItemAtFast(index));
				ShowWindowWithPortfolio(portfolio);
			}
			break;
		}

		case kRenamePortfolioMessage: {

			BString portfolioName;
			if (message->FindString("PortfolioName", &portfolioName) != B_OK ) {
				return;
			}

			if (Portfolio *portfolio = CurrentSelectedPortfolio()) {
				portfolio->SetName(portfolioName);
				fPortfolioManager->Save();
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
