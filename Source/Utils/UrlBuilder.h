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

class BList;
class UrlBuilder {
public:
	UrlBuilder(const char *base);
	~UrlBuilder();
	
	void RemoveSymbol(const char *symbol);
	void AddSymbol(const char *symbol);
	void MakeEmpty();
	
	bool IsBatchRequestValid() const;
	const char *CreateBatchPath();	
	
	void SetCompany(const char*name);
	const char *CreateCompanyPath(RequestType type);
		
private:

	int32 	IndexOfSymbol(const char *symbol);
	bool	HasSymbol(const char *symbol);

	BString fBaseUrl;
	BString fBuffer; 
	BString fBatchBuffer;
	BList *fSymbolList;
};

#endif // _H
