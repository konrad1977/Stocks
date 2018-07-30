/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ListItemDrawer.h"

#include <Screen.h>

ListItemDrawer::ListItemDrawer(BView *parent, bool isReplicant)
	:fParent(parent)
	,fIsReplicant(isReplicant)
	,fInsets(BSize(5,5)) {

}
	
ListItemDrawer::~ListItemDrawer() {
	
}

void 
ListItemDrawer::SetInsets(BSize insets) {
	fInsets = insets;
}

rgb_color
ListItemDrawer::BackgroundColor(bool isSelected) {
	if (fIsReplicant && isSelected == false) {
		BScreen screen;		
		rgb_color color = screen.DesktopColor();
		color.alpha = 140;
		return color;
	}
	return ui_color(B_LIST_BACKGROUND_COLOR);
}

rgb_color
ListItemDrawer::TextColor(bool isSelected) {

	if (fIsReplicant) {
		rgb_color backgroundColor = BackgroundColor(isSelected);
		if ( backgroundColor.red < 127 || backgroundColor.green < 127 ||  backgroundColor.blue < 127) {
			rgb_color textColor = { 224, 220, 224 };
			return textColor;
		}
	}
	return ui_color( isSelected ? B_LIST_SELECTED_ITEM_TEXT_COLOR : B_LIST_ITEM_TEXT_COLOR);
}

void 
ListItemDrawer::DrawString(const char *text, DrawItemSettings settings) {
	const BFont *font = settings.font == NULL ? be_plain_font : settings.font;
	DrawString(text, settings.frame, font, settings.align, settings.color);
}

float ListItemDrawer::Height(DrawItemSettings settings) {
	if (settings.font == NULL) {
		return settings.frame.Height();
	}
	
	font_height fh;
	settings.font->GetHeight(&fh);
	return fh.ascent + fh.descent + fh.leading;
}

void 
ListItemDrawer::DrawString(const char *text, BRect frame, const BFont *font, alignment align, rgb_color *color) {
		
	fParent->SetFont(font);
	
	font_height fh;
	font->GetHeight(&fh);
	
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float horizontalCenter = ((frame.Height() - fontHeight) / 2) + fh.descent;

	switch (align) {
		case B_ALIGN_LEFT: {
			fParent->MovePenTo( fInsets.width, frame.LeftBottom().y - horizontalCenter);	
			break;
		}		
		case B_ALIGN_RIGHT: {
			const float width = font->StringWidth(text);
			fParent->MovePenTo( frame.RightBottom().x - width - fInsets.width, frame.RightBottom().y - horizontalCenter);	
			break;
		}
		case B_ALIGN_CENTER: {
			const float width = font->StringWidth(text);
			const float center = (frame.Width() - width) / 2.0;
			fParent->MovePenTo( center, frame.RightBottom().y - horizontalCenter);	
			break;
		}
	}
	
	if (color == NULL) {
		fParent->SetHighColor(TextColor(false));		
	} else {
		fParent->SetHighColor(*color);
	}
	fParent->DrawString(text);
}