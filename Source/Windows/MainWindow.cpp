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
#include "Portfolio.h"

#include <Alert.h>
#include <Application.h>
#include <Catalog.h>

#include <Messenger.h>
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

MainWindow::MainWindow(Portfolio *portfolio)
	:BWindow(BRect(30,30, 1, 1), B_TRANSLATE("Portfolio"), B_TITLED_WINDOW, B_FRAME_EVENTS | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
	,fMenuBar(NULL)
	,fContainerView(NULL)
	,fSettingsWindow(NULL)
	,fRemoveSelected(NULL)
	,fMinimalItem(NULL)
	,fNormalItem(NULL)
	,fExtenededItem(NULL)
	,fMessenger(NULL)
	,fPortfolio(portfolio)
{
	SetupViews();
	SetTitle( fPortfolio->Name() );
	InitQuoteType();
}

MainWindow::~MainWindow()
{
	delete fMessenger;
}

void
MainWindow::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

SettingsWindow*
MainWindow::CurrentSettingWindow()
{
	if (fSettingsWindow == NULL) {
		fSettingsWindow = new SettingsWindow(fPortfolio);
		fSettingsWindow->SetTarget(this);
	}
	return fSettingsWindow;
}

void
MainWindow::SendToContainerView(BMessage *message)
{
	BMessenger messenger(fContainerView);
	messenger.SendMessage(message);
}

void
MainWindow::SendSaveMessage()
{
	BMessage message(kPortfolioManagerSaveMessage);
	fMessenger->SendMessage(&message);
}

void
MainWindow::SetSelectedMenuFromQuoteType(QuoteType type)
{
	fMinimalItem->SetMarked(false);
	fNormalItem->SetMarked(false);
	fExtenededItem->SetMarked(false);

	switch (type) {
		case SMALL_TYPE:
			fMinimalItem->SetMarked(true);
			break;
		case NORMAL_TYPE:
			fNormalItem->SetMarked(true);
			break;
		case LARGE_TYPE:
			fExtenededItem->SetMarked(true);
		default:
			break;
	}
}

void
MainWindow::InitQuoteType()
{
	SetSelectedMenuFromQuoteType(fPortfolio->CurrentQuoteType());
}

void
MainWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {

		case kListSelectMessage: {
			int32 index;
			if (message->FindInt32("index", &index) == B_OK) {
				fRemoveSelected->SetEnabled(index != -1);
			}
			break;
		}
		case kListInvocationMessage: {
			printf("Show stock information?\n");
			break;
		}

		case kRemoveSelectedListItem:
		case kPortfolioButtonPressedMessage:
		case B_ABOUT_REQUESTED: {
			SendToContainerView(message);
			break;
		}

		case kUseSmallQuoteSize: {
			SendToContainerView(message);
			SetSelectedMenuFromQuoteType(SMALL_TYPE);
			break;
		}

		case kUseNormalQuoteSize: {
			SendToContainerView(message);
			SetSelectedMenuFromQuoteType(NORMAL_TYPE);
			break;
		}

		case kUseLargeQuoteSize: {
			SendToContainerView(message);
			SetSelectedMenuFromQuoteType(LARGE_TYPE);
			break;
		}

		case kQuitSettingsWindowMessage: {
			fSettingsWindow = NULL;
			break;
		}

		case kShowSettingsWindowMessage: {
			CurrentSettingWindow()->ShowWithPortfolioName(fPortfolio->Name().String());
			break;
		}

		case kPortfolioUpdatedSettingsMessage: {
			SendSaveMessage();
			break;
		}

		case kHideSearchWindowMessaage: {
			fMessenger->SendMessage(message);
			break;
		}

		case kShowSearchWindowMessage: {
			BMessage message(kShowSearchWindowMessage);
			message.AddString("PortfolioName", Title());
			fMessenger->SendMessage(&message);
			break;
		}

		default:
			BWindow::MessageReceived(message);
			break;
	}
}

void
MainWindow::SetupViews()
{
	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	layout->SetSpacing(0);
	SetLayout(layout);

	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
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

	fContainerView = new ContainerView(fPortfolio);
	fContainerView->SetExplicitMinSize(BSize(220, B_SIZE_UNSET));
	fContainerView->SetExplicitMaxSize(BSize(680, B_SIZE_UNSET));
	fContainerView->SetTarget(this);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fContainerView);

	fRemoveSelected->SetEnabled(false);
}
