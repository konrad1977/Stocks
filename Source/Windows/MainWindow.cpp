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
#include "SettingsWindow.h"

#include <Alert.h>
#include <Application.h>
#include <Catalog.h>

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

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"

MainWindow::MainWindow(BRect rect) 
	:BWindow(rect, B_TRANSLATE("Portfolio"), B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE)
	,fMenuBar(NULL) 
	,fContainerView(NULL)
	,fStockRequester(NULL)
	,fStockSymbolWindow(NULL)
	,fSettingsWindow(NULL)
	,fSymbolList(NULL)
	,fShowStockSymbolListWhenDone(false)
	,fStockSymbolsLoaded(false)	
	,fRemoveSelected(NULL)
	,fMinimalItem(NULL)
	,fNormalItem(NULL)
	,fExtenededItem(NULL) {
 	
	SetupViews();
	InitQuoteSize();
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
MainWindow::ShowStockWindow() {
	if (SymbolWindow()->IsHidden() || SymbolWindow()->IsMinimized()) {
		SymbolWindow()->SetStockSymbols(fSymbolList);
		SymbolWindow()->Show();				
	}
}

SettingsWindow*
MainWindow::CurrentSettingWindow() {
	if (fSettingsWindow == NULL) {
		fSettingsWindow = new SettingsWindow();
		fSettingsWindow->SetTarget(this);
	}
	return fSettingsWindow;
}

void
MainWindow::SendToContainerView(BMessage *message) {
	BMessenger messenger(fContainerView);
	messenger.SendMessage(message);
}

void
MainWindow::SetSelectedMenuFromQuoteSize(QuoteSize size) {
	
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
	SetSelectedMenuFromQuoteSize(SettingsManager().CurrentQuoteSize());
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
			SetSelectedMenuFromQuoteSize(SMALL);
			break;
		}

		case kUseNormalQuoteSize: {
			SendToContainerView(message);
			SetSelectedMenuFromQuoteSize(NORMAL);
			break;
		}

		case kUseLargeQuoteSize: {
			SendToContainerView(message);
			SetSelectedMenuFromQuoteSize(LARGE);
			break;
		}
		
		case kQuitSettingsWindowMessage: {
			fSettingsWindow = NULL;
			break;
		}

		case kShowSettingsWindowMessage: {
			CurrentSettingWindow()->Show();			
			break;
		}
		
		case kUpdateSymbolMessage: { 
			HandleStockSearchSymbols(message);
			break;		
		}
		
		case kHideSearchWindowMessaage: {
			fStockSymbolWindow = NULL;
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

void
MainWindow::SetupViews() {

	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	layout->SetSpacing(0);
	SetLayout(layout);
	
	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(B_TRANSLATE("About..."), B_ABOUT_REQUESTED, 'A')
			.AddSeparator()
			.AddItem(B_TRANSLATE("Quit"), B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu(B_TRANSLATE("Edit"))
			.AddItem(fRemoveSelected = new BMenuItem(B_TRANSLATE("Remove selected item"), new BMessage(kRemoveSelectedListItem), 'R'))
		.End()
		.AddMenu(B_TRANSLATE("Settings"))
			.AddItem(B_TRANSLATE("Find stocks..."), kShowSearchWindowMessage, 'F')
			.AddSeparator()
			.AddItem(fMinimalItem = new BMenuItem(B_TRANSLATE("Mini mode"), new BMessage(kUseSmallQuoteSize), '1'))
			.AddItem(fNormalItem = new BMenuItem(B_TRANSLATE("Normal mode"), new BMessage(kUseNormalQuoteSize), '2'))
			.AddItem(fExtenededItem = new BMenuItem(B_TRANSLATE("Extended mode"), new BMessage(kUseLargeQuoteSize), '3'))
			.AddSeparator()
			.AddItem(B_TRANSLATE("Settings..."), kShowSettingsWindowMessage, 'S')
		.End();
	
	fContainerView = new ContainerView();
	fContainerView->SetTarget(this);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fContainerView);
}
