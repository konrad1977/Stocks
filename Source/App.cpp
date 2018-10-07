/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "MainWindow.h"
#include "PortfolioManager.h"
#include "PortfolioManagerWindow.h"
#include "Portfolio.h"
#include "ImageView.h"

#include <stdio.h>
#include <stdlib.h>
#include <AboutWindow.h>
#include <Catalog.h>
#include <Locale.h>


const char *kAppSignature = "application/x-vnd.konradsson.Stocks";

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "App"

App::App(void)
	:BApplication(kAppSignature)
{
	PortfolioManagerWindow *window = new PortfolioManagerWindow();
	window->Show();
}

App::~App() {

}

void
App::AboutRequested()  {

	const char* specialThanks[] = {
		"Humdinger for the lovely Icon",
		NULL
	};

	BAboutWindow* window = new BAboutWindow("Stocks", kAppSignature);
	window->AddCopyright(2018, "Mikael Konradsson");
	window->AddDescription("Simple application for creating custom portfolios that can be added to your desktop as replicants.");
	window->AddSpecialThanks(specialThanks);
	window->AddExtraInfo("Distributed on MIT license terms.");
	window->Show();
}

void
App::MessageReceived(BMessage *message) {
	message->PrintToStream();
}

int main() {
	App app;
	app.Run();
	return 0;
}
