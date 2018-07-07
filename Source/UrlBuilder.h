/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef URLBUILDER_H
#define URLBUILDER_H

#include <SupportDefs.h>
#include <String.h>

enum RequestType {
	Company = 0,
	Chart,
	News,
	Logo
};	

class UrlBuilder {
public:
	UrlBuilder(const char *base);
	void SetCompany(const char*name);
	const char *CreatePath(RequestType type);
		
private:
	BString fBaseUrl;
	BString fBuffer; 
};

#endif // _H
