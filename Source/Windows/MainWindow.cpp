/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"
#include "StockRequester.h"
#include "MessageConstants.h"
#include "ContainerView.h"

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
	:BWindow(rect, "Portfolio", B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE )
	,fMenuBar(NULL) {
	SetupViews();
}

MainWindow::~MainWindow() {

}

void
MainWindow::SetupViews() {

	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	SetLayout(layout);
	
	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu("Main")
			.AddItem("About...", B_ABOUT_REQUESTED, 'A')
			.AddSeparator()
			.AddItem("Quit", B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu("Settings")
			.AddItem("Add symbol...", kShowSearchWindowMessage, 'S')
		.End();
	
	fContainerView = new ContainerView();
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(0,0,0,0)
		.Add(fMenuBar)
		.Add(fContainerView);		
}

void
MainWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
			
		case kShowSearchWindowMessage: {
			BMessage *showSearchWindowMessage = new BMessage(kShowSearchWindowMessage);
			be_app_messenger.SendMessage(showSearchWindowMessage);
			delete showSearchWindowMessage;
			break;
		}
		
		case B_ABOUT_REQUESTED:
			break;
			
		case kUpdateCompanyMessage: {			
			printf("kUpdateCompanyMessage\n");
			/*BMessage companyMessage;
			message->FindMessage("Company", &companyMessage);
			Company *company = new Company(companyMessage);
			fStockListView->AddItem(new BStringItem(company->name));*/
			break;
		}
		default:
			break;
	}
}

