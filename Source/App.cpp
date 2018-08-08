/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "MainWindow.h"
#include "PortfolioManager.h"
#include "Portfolio.h"
#include <stdio.h>


const char *kAppSignature = "application/x-vnd.konradsson.Stocks";

App::App(void)
	:BApplication(kAppSignature)
	,fPortfolioManager(NULL) {
	
	fPortfolioManager = new PortfolioManager(this);
	
	//fWindow = new MainWindow(BRect(150,150,420,540));
	//fWindow->Show();		
		
	/*Portfolio *portfolio1 = new Portfolio("Portfolio 1");
	portfolio1->Add("AAPL");
	portfolio1->Add("GOOG");

	Portfolio *portfolio2 = new Portfolio("Portfolio 2");
	portfolio2->Add("SONO");
	
	PortfolioManager *manager = new PortfolioManager();*/
	/*manager->AddPortfolio(portfolio1);
	manager->AddPortfolio(portfolio2);
	manager->Save();*/
}

App::~App() {
	delete fPortfolioManager;
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
