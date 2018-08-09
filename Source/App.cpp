/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "MainWindow.h"
#include "PortfolioManager.h"
#include "PortfolioManagerWindow.h"
#include "Portfolio.h"
#include <stdio.h>


const char *kAppSignature = "application/x-vnd.konradsson.Stocks";

App::App(void)
	:BApplication(kAppSignature)
{		
	PortfolioManagerWindow *window = new PortfolioManagerWindow();	
	window->Show();
}

App::~App() {

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
