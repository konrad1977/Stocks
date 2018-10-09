/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H


#include <SupportDefs.h>
#include <support/String.h>
#include <Window.h>

class Portfolio;
class BSlider;
class BMessenger;
class BHandler;
class SettingsWindow : public BWindow {
public:
	SettingsWindow(Portfolio *portfolio);
	~SettingsWindow();

			void ShowWithPortfolioName(const char *name);
	virtual void MessageReceived(BMessage *message);
			void Quit();
			void SetTarget(BHandler *handler);
private:
			void InitLayout();
			void InitSavedValues();
			void UpdateTransparencyLabel(uint8 value);
			void UpdateRefrehLabel(uint8 value);

	BMessenger 	*fMessenger;
	BSlider 	*fTransparencySlider;
	BSlider 	*fRefreshRateSlider;
	Portfolio	*fPortfolio;
	BString 	fTransparencyLabel;
	BString 	fRefreshLabel;

};


#endif // _H
