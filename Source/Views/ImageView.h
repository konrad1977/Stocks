/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H


#include <SupportDefs.h>
#include <support/String.h>
#include <View.h>

class ImageView: public BView {
public:

	ImageView(BRect rect, const BString& url);
	~ImageView();
	
	virtual void AttachedToWindow();
	virtual void Draw(BRect rect);
	
private:
	status_t DownloadImage(BDataIO* stream);
	BString fUrl;
	BBitmap *fBitmap;
};

#endif // _H
