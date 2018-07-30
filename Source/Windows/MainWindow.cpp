/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"
#include "Constants.h"
#include "ContainerView.h"
#include "StockSymbolWindow.h"
#include "SymbolListItem.h"
#include "StockRequester.h"

#include <Application.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Locale.h>

#include <LayoutBuilder.h>
#include <stdio.h>

#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

MainWindow::MainWindow(BRect rect) 
	:BWindow(rect, "Portfolio", B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE)
	,fMenuBar(NULL) 
	,fSymbolList(NULL)
	,fStockRequester(NULL)
	,fStockSymbolWindow(NULL)
	,fShowStockSymbolListWhenDone(false)
	,fStockSymbolsLoaded(false)	
{ 	
	SetupViews();
	DownloadStockSymbols();
}

MainWindow::~MainWindow() {
	delete fStockRequester;
}

void 
MainWindow::DownloadStockSymbols() {
	if (fStockRequester == NULL) {
		fStockRequester = new StockRequester(this);
	}
	fStockRequester->DownloadSymbols();
}

StockSymbolWindow *
MainWindow::SymbolWindow() {
	if (fStockSymbolWindow == NULL) {		
		fStockSymbolWindow = new StockSymbolWindow();
		fStockSymbolWindow->SetTarget(this);	
	}
	return fStockSymbolWindow;
}

void
MainWindow::SetupViews() {

	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	layout->SetSpacing(0);
	SetLayout(layout);
	
	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu("Main")
			.AddItem("About...", B_ABOUT_REQUESTED, 'A')
			.AddSeparator()
			.AddItem("Quit", B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu("Edit")
			.AddItem("Remove selected item", kRemoveSelectedListItem, 'R')
			.AddSeparator()
			.AddItem("Use small size", kUseSmallQuoteSize, 'S')
			.AddItem("Use normal size", kUseNormalQuoteSize, 'N')
			.AddItem("Use large size", kUseLargeQuoteSize, 'L')
		.End()
		.AddMenu("Settings")
			.AddItem("Find symbols...", kShowSearchWindowMessage, 'F')
		.End();
	
	fContainerView = new ContainerView();
	fContainerView->SetTarget(this);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fContainerView);
}

void
MainWindow::ShowStockWindow() {
	if (SymbolWindow()->IsHidden() || SymbolWindow()->IsMinimized()) {
		SymbolWindow()->SetStockSymbols(fSymbolList);
		SymbolWindow()->Show();				
	}
}

void
MainWindow::SendToContainerView(BMessage *message) {
	BMessenger *messenger = new BMessenger(fContainerView);
	messenger->SendMessage(message);
	delete messenger;
}

void
MainWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
	
		case kRemoveSelectedListItem:
		case kPortfolioButtonPressedMessage:
		case kUseSmallQuoteSize:
		case kUseNormalQuoteSize:
		case kUseLargeQuoteSize: {
			SendToContainerView(message);
			break;
		}
		
		case kUpdateSymbolMessage: { 
			HandleStockSearchSymbols(message);
			break;		
		}
		
		case kHideSearchWindowMessaage: {
			fStockSymbolWindow = NULL;
			printf("kHideSearchWindowMessaage\n");
			break;
		}
		
		case kEmptyListMessage: {
			if (fStockSymbolsLoaded) {
				ShowStockWindow();
				return;
			}
			fShowStockSymbolListWhenDone = true;
			break;
		}
		case kShowSearchWindowMessage: {
			ShowStockWindow();
			break;
		}
		
		case B_ABOUT_REQUESTED:
			break;
	
		default:
			break;
	}
}

void 
MainWindow::HandleStockSearchSymbols(BMessage *message) {
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

