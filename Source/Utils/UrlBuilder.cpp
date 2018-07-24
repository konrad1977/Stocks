/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "UrlBuilder.h"

#include <List.h>

#include <posix/string.h>

UrlBuilder::UrlBuilder(const char *base)
	:fBaseUrl(BString(base)) 
	,fSymbolList(NULL) {
	
	fSymbolList = new BList();
}

UrlBuilder::~UrlBuilder() {
	delete fSymbolList;
}

int32 
UrlBuilder::IndexOfSymbol(const char *symbol) {

	if (fSymbolList == NULL || fSymbolList->IsEmpty())
		return -1;
	
	for (int32 i = 0; i<fSymbolList->CountItems(); i++) {
		char *sym = (char *)fSymbolList->ItemAtFast(i);
		if (strcasecmp(sym, symbol) == 0) {
			return i;
		}
	}
	return -1;
}

const char *
UrlBuilder::CreateBatchPath() {

	fBatchBuffer = fBaseUrl;
	fBatchBuffer << "/market/batch?symbols=";
	
	BString format;
	const int32 count = fSymbolList->CountItems();
	for (int32 i = 0; i<count; i++) {
		char *symbol = (char *)fSymbolList->ItemAt(i);
		if (i == count-1) {
			format << symbol;
		} else {
			format << symbol << ",";
		}		
	}
	fBatchBuffer << format << "&types=quote";
	return fBatchBuffer;
}

void
UrlBuilder::RemoveSymbol(const char *symbol) {
	int32 index = IndexOfSymbol(symbol);
	if (index != -1) {
		fSymbolList->RemoveItem(index);
	}
}

void
UrlBuilder::MakeEmpty() {
	fSymbolList->MakeEmpty();
}

bool
UrlBuilder::HasSymbol(const char *symbol) {
	return IndexOfSymbol(symbol) != -1;
}
void
UrlBuilder::AddSymbol(const char *symbol) {
	if (HasSymbol(symbol)) {
		return;
	}
	fSymbolList->AddItem((void*)symbol);
}

void
UrlBuilder::SetCompany(const char*name) {
	fBuffer = fBaseUrl;
	fBuffer << "/" << name;
}

const char *
UrlBuilder::CreateCompanyPath(RequestType type) {

	switch (type) {
		case Company:
			fBuffer << "/batch?types=quote,company";
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
