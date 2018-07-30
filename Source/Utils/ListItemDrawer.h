/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef LISTITEMDRAWER_H
#define LISTITEMDRAWER_H


#include <SupportDefs.h>
#include <InterfaceDefs.h>
#include <View.h>
#include <Font.h>

struct DrawItemSettings {
	BRect frame;
	BFont *font;
	rgb_color *color;
	alignment align;
};

class ListItemDrawer {
public:
	ListItemDrawer(BView *parent, bool isReplicant);
	~ListItemDrawer();
	
	rgb_color BackgroundColor(bool IsSelected);
	rgb_color TextColor(bool isSelected);
	
	void SetInsets(BSize insets);
	float Height(DrawItemSettings settings);
	void DrawString(const char *text, DrawItemSettings settings);
	
private:

	void DrawString(const char *text, BRect frame, const BFont *font, alignment align, rgb_color *color = NULL);

	BView 	*fParent;
	BSize 	fInsets;
	bool 	fIsReplicant;
};


#endif // _H
