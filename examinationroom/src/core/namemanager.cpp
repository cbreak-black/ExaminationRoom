/*
 *  namemanager.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 12.05.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "namemanager.h"

#include <sstream>

namespace Examination
{

// NamedItem

NamedItem::NamedItem(const std::string & name)
{
	name_ = name;
}

NamedItem::~NamedItem()
{
}

std::string NamedItem::name() const
{
	return name_;
}

// NamedObject
NamedObject::NamedObject(const std::string & name, std::tr1::shared_ptr<Object> object)
	: NamedItem(name)
{
	object_ = object;
}

std::tr1::shared_ptr<Object> NamedObject::object() const
{
	return object_.lock();
}

// NameManager

NameManager::NameManager()
{
}

// Unique Name Management
const char saneCharacters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
const char saneCharactersFirst[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

bool NameManager::checkUniqueName(const std::string & name) const
{
	return names_.find(name) == names_.end();
}

std::string NameManager::uniqueName(const std::string & name) const
{
	std::string tempName = name;
	// Search a free name
	int i = 1;
	while (!checkUniqueName(tempName))
	{
		std::stringstream ss(name, std::ios_base::out | std::ios_base::app);
		ss << i;
		tempName = ss.str();
		i++;
	}
	return tempName;
}

std::string NameManager::sanitizeName(const std::string & name)
{
	std::string saneName = name;
	if (name.size() == 0)
	{
		return "EmptyString";
	}
	size_t pos = saneName.find_last_not_of(saneCharacters);
	while (pos != std::string::npos)
	{
		saneName[pos] = '_';
		pos = saneName.find_last_not_of(saneCharacters, pos);
	}
	pos = saneName.find_last_not_of(saneCharactersFirst, 0);
	if (pos != std::string::npos)
	{
		saneName[0] = '_';
	}
	return saneName;
}

NamedItemPtr NameManager::registerName(const std::string & name)
{
	std::string tempName = uniqueName(name);
	// Since the name is free, register it
	NamedItemPtr tempNamedItem = NamedItemPtr(new NamedItem(tempName));
	names_.insert(std::pair<std::string,NamedItemPtr>(tempName, tempNamedItem));
	return tempNamedItem;
}

NamedItemPtr NameManager::registerName(const std::string & name, std::tr1::shared_ptr<Object> object)
{
	std::string tempName = uniqueName(name);
	// Since the name is free, register it
	NamedItemPtr tempNamedItem = NamedItemPtr(new NamedObject(tempName, object));
	names_.insert(std::pair<std::string,NamedItemPtr>(tempName, tempNamedItem));
	return tempNamedItem;
}

NamedItemPtr NameManager::registerName(NamedItemPtr namedItem)
{
	if (checkUniqueName(namedItem->name()))
	{
		names_.insert(std::pair<std::string,NamedItemPtr>(namedItem->name(), namedItem));
		return namedItem;
	}
	else
	{
		return NamedItemPtr();
	}
}

void NameManager::unregisterName(const std::string & name)
{
	names_.erase(name);
}

const char * luaKeywords[] =
{
	"and",	"break",	"do",	"else",	"elseif",
	"end",	"false",	"for",	"function",	"if",
	"in",	"local",	"nil",	"not",	"or", "repeat",
	"return",	"then",	"true",	"until",	"while",
	0
};

void NameManager::registerLuaKeywords()
{
	for (unsigned int i = 0; luaKeywords[i]; i++)
	{
		registerName(luaKeywords[i]);
	}
}


}
