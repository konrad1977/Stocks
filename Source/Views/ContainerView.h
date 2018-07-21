/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CONTAINERVIEW_H
#define CONTAINERVIEW_H

#include <SupportDefs.h>
#include <View.h>

class BDragger;
class ContainerView : public BView {
public:
	ContainerView(BRect rect);
	ContainerView(BMessage *archive);
	~ContainerView();

	virtual status_t	Archive(BMessage* into, bool deep = true) const;
	static BArchivable*	Instantiate(BMessage* archive);
			status_t	SaveState(BMessage* into, bool deep = true) const;
 
	
private:
		void SetupViews();
	BDragger *fDragger;
};

#endif // _H
