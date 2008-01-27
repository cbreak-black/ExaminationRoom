/*
 *  refcountable.cpp
 *  ExaminationRoom
 *
 *  Created by CBreak on 27.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "refcountable.h"

#include <assert.h>

namespace Tool
{

RefCountable::RefCountable()
{
	refCounter_ = 1;
}

RefCountable::~RefCountable()
{
	assert(refCounter_ == 0); // Instance was deleted manualy
}

int RefCountable::retain()
{
	return (refCounter_++);
}

int RefCountable::release()
{
	refCounter_--;
	if (refCounter_ == 0)
		delete this;
	return refCounter_;
}

}