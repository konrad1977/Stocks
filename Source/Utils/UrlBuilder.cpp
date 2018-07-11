/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "UrlBuilder.h"

UrlBuilder::UrlBuilder(const char *base) {
	fBaseUrl = BString(base);
}

void
UrlBuilder::SetCompany(const char*name) {
	fBuffer = fBaseUrl;
	fBuffer << "/" << name;
}

const char *
UrlBuilder::CreatePath(RequestType type) {

	switch (type) {
		case Company:
			fBuffer << "/company";
			break;
		case Chart:
			fBuffer << "/chart";
			break;
		case News:
			fBuffer << "/news";
			break;
		case Logo: 
			fBuffer << "/logo";	
		default:
			break;
	}	
	return fBuffer.String();	
}
