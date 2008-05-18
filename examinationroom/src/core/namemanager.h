/*
 *  namemanager.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 12.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NAMEMANAGER_H
#define NAMEMANAGER_H

#include <string>
#include <memory>
#include <map>

namespace Examination
{
	class Object;

/**
A helper class that is used to store different things within the name manager.
Subclasses are used to store additional things.
*/
class NamedItem
{
public:
	/**
	Creates a named item with the given name.
	*/
	NamedItem(const std::string & name);

	/**
	Virtual empty destructor
	*/
	virtual ~NamedItem();

public: // Accessors
	/**
	Returns the name of this item.
	 \return the name of this item
	*/
	std::string name() const;

private:
	std::string name_;
};

/**
A helper class that stores an object pointer as NamedItem
*/
class NamedObject : public NamedItem
{
public:
	NamedObject(const std::string & name, std::tr1::shared_ptr<Object> object);
public:
	std::tr1::shared_ptr<Object> object() const;
private:
	std::tr1::weak_ptr<Object> object_;
};

/**
A helper class that stores a class as NamedItem
*/
template <typename O>
class NamedClass : public NamedItem
{
public:
	NamedClass(const std::string & name) : NamedItem(name)
	{
	}
public:
	typedef O classType;
};

/**
This type is used as shorthand. All named items should always be pointed at
by a shared_ptr, and are not intended to be used by-value, this type is the
intended way to refer to them.
Polymorphism does not work with value types.
*/
typedef std::tr1::shared_ptr<NamedItem> NamedItemPtr;


/**
This class stores items by name. Names are unique per instance.
*/
class NameManager
{
public:
	/**
	Creates an empty name manager.
	*/
	NameManager();

public: // Name checking
	/**
	Checks if the passed name is available.
	 \param name	The desired name
	 \return True if the passed name is available, false if it would have to be changed
	*/
	bool checkUniqueName(const std::string & name) const;

	/**
	Returns a unique version of the passed name.
	The unique name is built by appending numbers, and is guaranteed to not exist
	in this NameManager.
	 \param name	The base name
	 \return A unique version of the passed name
	*/
	std::string uniqueName(const std::string & name) const;

	/**
	Sanitizes a name, removing evil characters.
	 \param name	A name containing potentially evil characters
	 \return	The name without those characters
	*/
	static std::string sanitizeName(const std::string & name);

public: // Name Registering
	/**
	Registers a unique name with the object class. If the passed name is unique,
	it is registered and is and returned unchanged.
	If the passed name is not unique, a unique version is built by appending
	numbers.
	 \param name	The desired base name
	 \return a named item containing the unique version of the name
	*/
	NamedItemPtr registerName(const std::string & name);

	/**
	Registers a unique name with the object class. If the passed name is unique,
	it is registered and is and returned unchanged.
	If the passed name is not unique, a unique version is built by appending
	numbers.
	The passed object is added to the NamedItem by using the subclass NamedObject.
	This instance is returned.
	 \param name	The desired base name
	 \return a named item containing the unique version of the name
	*/
	NamedItemPtr registerName(const std::string & name, std::tr1::shared_ptr<Object> object);

	/**
	Registers a unique name with the object class. If the passed name is unique,
	it is registered at is and returned unchanged.
	If the passed name is not unique, an invalid pointer is returned
	 \param namedItem	A named item pointer
	 \return the named item that was registered, or an invalid pointer
	*/
	NamedItemPtr registerName(NamedItemPtr namedItem);

	/**
	Unregisters a name from the object class.
	 \param name	The actual name
	*/
	void unregisterName(const std::string & name);

private:
	std::map<std::string,NamedItemPtr> names_;
};

}

#endif