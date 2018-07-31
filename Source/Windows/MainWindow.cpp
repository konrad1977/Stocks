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
#include "SettingsManager.h"

#include <Alert.h>
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
	,fRemoveSelected(NULL)
	,fMinimalItem(NULL)
	,fNormalItem(NULL)
	,fExtenededItem(NULL)
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
			.AddItem(fRemoveSelected = new BMenuItem("Remove selected item", new BMessage(kRemoveSelectedListItem), 'R'))
		.End()
		.AddMenu("Settings")
			.AddItem("Find stocks...", kShowSearchWindowMessage, 'F')
			.AddSeparator()
			.AddItem(fMinimalItem = new BMenuItem("Minimal quote information", new BMessage(kUseSmallQuoteSize), 'S'))
			.AddItem(fNormalItem = new BMenuItem("Normal quote information", new BMessage(kUseNormalQuoteSize), 'N'))
			.AddItem(fExtenededItem = new BMenuItem("Extended quote information", new BMessage(kUseLargeQuoteSize), 'L'))
		.End();
	
	fContainerView = new ContainerView();
	fContainerView->SetTarget(this);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fContainerView);
		
	InitQuoteSize();
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
MainWindow::SetQuoteSize(QuoteSize size) {
	
	fMinimalItem->SetMarked(false);
	fNormalItem->SetMarked(false);
	fExtenededItem->SetMarked(false);
	
	switch (size) {
		case SMALL:
			fMinimalItem->SetMarked(true);
			break;
		case NORMAL:
			fNormalItem->SetMarked(true);
			break;
		case LARGE:
			fExtenededItem->SetMarked(true);
	}
}

void 
MainWindow::InitQuoteSize() {
	SettingsManager manager;
	QuoteSize size = manager.CurrentQuoteSize();
	SetQuoteSize(size);
}

void
MainWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
	
		case kRemoveSelectedListItem:
		case kPortfolioButtonPressedMessage:
		case B_ABOUT_REQUESTED: {
			SendToContainerView(message);
			break;
		}
		
		case kUseSmallQuoteSize: {
			SendToContainerView(message);
			SetQuoteSize(SMALL);
			break;
		}

		case kUseNormalQuoteSize: {
			SendToContainerView(message);
			SetQuoteSize(NORMAL);
			break;
		}

		case kUseLargeQuoteSize: {
			SendToContainerView(message);
			SetQuoteSize(LARGE);
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
			
		default:
			BWindow::MessageReceived(message);
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

