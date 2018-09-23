/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ImageView.h"
#include <Bitmap.h>
#include <Url.h>
#include <UrlContext.h>
#include <UrlProtocolListener.h>
#include <UrlProtocolRoster.h>
#include <HttpRequest.h>
#include <HttpHeaders.h>
#include <TranslationUtils.h>

#include <stdio.h>

class ProtocolListener: public BUrlProtocolListener {
public:
	ProtocolListener()
		:fDownloadIO(NULL) {}
	
	virtual ~ProtocolListener()	{ } 
	virtual void DataReceived(BUrlRequest* caller, const char* data, off_t position, ssize_t size) 
	{
		if (fDownloadIO != NULL) {
			fDownloadIO->Write(data, size);
		}
	}
	
	void SetDownloadIO(BDataIO* downloadIO) 
	{
		fDownloadIO = downloadIO;
	}

private:
	BDataIO*		fDownloadIO;
};

ImageView::ImageView(BRect rect, const BString &url) 
	:BView(rect, "ImageView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
	,fUrl(url)
	,fBitmap(NULL)
{
}

ImageView::~ImageView()
{

}

void 
ImageView::Draw(BRect rect) 
{
	if (fBitmap == NULL) {
		return;
	}
	
	BRect r(Bounds());
	SetDrawingMode(B_OP_ALPHA);
	DrawBitmap(fBitmap, fBitmap->Bounds(), r, B_FILTER_BITMAP_BILINEAR);
	SetDrawingMode(B_OP_COPY);

	BView::Draw(rect);
}

void 
ImageView::AttachedToWindow()
{
	BMallocIO buffer;
	if (DownloadImage(&buffer) == B_OK) {
		if (fBitmap = BTranslationUtils::GetBitmap(&buffer)) {
			Invalidate();
		}
	}
	BView::AttachedToWindow();
}

status_t
ImageView::DownloadImage(BDataIO* stream) 
{
	BUrl url(fUrl);
	bool isSecure = url.Protocol() == "https";
	
	ProtocolListener listener;
	listener.SetDownloadIO(stream);
	
	BHttpRequest request(url, isSecure, "HTTP", &listener);
	request.SetMethod(B_HTTP_GET);
	
	thread_id thread = request.Run();
	wait_for_thread(thread, NULL);

	const BHttpResult& result = dynamic_cast<const BHttpResult&>(request.Result());
	const int32 statusCode = result.StatusCode();

	if (statusCode == 200)
		return B_OK;
	return B_ERROR;
}