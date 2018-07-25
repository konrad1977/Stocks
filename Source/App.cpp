/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "MainWindow.h"
#include <stdio.h>

const char *kAppSignature = "application/x-vnd.konradsson.HaikuStocks";

App::App(void)
	:BApplication(kAppSignature) {
	
	fWindow = new MainWindow(BRect(150,150,420,540));
	fWindow->Show();		
}

App::~App() {

}

int main() {
	App app;
	app.Run();
	return 0; 	
}
