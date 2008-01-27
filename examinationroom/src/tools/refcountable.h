/*
 *  refcountable.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 27.01.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef REFCOUNTABLE_H
#define REFCOUNTABLE_H

namespace Tool
{

/**
This class provides an interface and implementation of a reference counting
memory management scheme. To keep a reference of such an object, retain it,
once it is time for disposal, release it.
*/
class RefCountable
{
public:
	/**
	The constructor automatically sets the reference count to 1.
	*/
	RefCountable();
	~RefCountable();
public:
	/**
	Increments the reference count by one.
	 \return the reference count after the retain
	*/
	int retain();
	
	/**
	Decrements the reference count by one. If the counter reaches zero, the
	Object is deleted.
	 \return the reference count after the release
	*/
	int release();

private:
	int refCounter_;
};

}
#endif