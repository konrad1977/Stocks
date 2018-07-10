/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef COMPANY_H
#define COMPANY_H


#include <SupportDefs.h>
#include <Message.h>
#include <String.h>

class Company {
public:
	Company(BMessage message);
	~Company();
	
	BString name;
	BString symbol;
	BString exchange;
	BString industry;
	BString website;
	BString description;
	BString CEO;
	BString sector;
	
	void PrintToStream();
private:
};


#endif // _H
