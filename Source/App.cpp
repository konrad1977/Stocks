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
#include "ImageView.h"

const char *kAppSignature = "application/x-vnd.konradsson.Stocks";

App::App(void)
	:BApplication(kAppSignature)
{		
	BWindow *window = new BWindow(BRect(0,0,200,200), "WIndow", B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE);

	BString url("http://files.softicons.com/download/system-icons/beos-icons-by-studiotwentyeight/png/128/BeOS_Logo.png");
	ImageView *view = new ImageView(window->Bounds(), url);

	window->AddChild(view);
	window->Show();
		
	
	//PortfolioManagerWindow *window = new PortfolioManagerWindow();	
	//window->Show();	
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
