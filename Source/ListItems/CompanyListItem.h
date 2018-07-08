/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef COMPANYLISTITEM_H
#define COMPANYLISTITEM_H


#include <SupportDefs.h>
#include <ListItem.h>
#include "Company.h"

class CompanyListItem : public BListItem {
public:
	CompanyListItem(Company *company);
	~CompanyListItem();
	
	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
								
private:
	Company *fCompany;
};


#endif // _H
