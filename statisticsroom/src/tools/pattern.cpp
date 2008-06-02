/*
 *  pattern.cpp
 *  StatisticsRoom
 *
 *  Created by cbreak on 01.06.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "pattern.h"

#include <QTextStream>

#include <iostream>

namespace Statistics
{

Pattern::Pattern(const QString & regExp, const QString & fieldNames)
{
	regExp_ = QRegExp(regExp);
	fieldNames_ = fieldNames.split(";");
	reset();
}

Pattern::Pattern(const QRegExp & regExp, const QStringList & fieldNames)
{
	regExp_ = regExp;
	fieldNames_ = fieldNames;
	reset();
}

QString Pattern::pattern() const
{
	return regExp_.pattern();
}

void Pattern::setPattern(const QString & pattern)
{
	regExp_ = QRegExp(pattern);
	reset();
}

QString Pattern::fieldNames() const
{
	return fieldNames_.join(";");
}

void Pattern::setFieldNames(const QString & fieldNames)
{
	fieldNames_ = fieldNames.split(";");
	reset();
}

bool Pattern::match(const QString & string)
{
	if (regExp_.exactMatch(string))
	{
		matched_++;
		fieldContents_.clear();
		for (int i = 0; i < fieldNames_.size(); i++)
		{
			fieldContents_.append(regExp_.cap(i+1));
		}
		return true;
	}
	else
	{
		return false;
	}
}

void Pattern::reset()
{
	matched_ = 0;
	fieldContents_.clear();
	for (int i = 0; i < fieldNames_.size(); i++)
	{
		fieldContents_.append("");
	}
}

void Pattern::printHeader(QTextStream & outStream)
{
	for (int i = 0; i < fieldNames_.size(); i++)
	{
		outStream << '\t' << fieldNames_[i];
	}
}

void Pattern::print(QTextStream & outStream)
{
	if (regExp_.numCaptures() > 0)
	{
		// Print the captures
		for (int i = 0; i < fieldContents_.size(); i++)
		{
			outStream << '\t' << fieldContents_[i];
			//fieldContents_[i] = ""; // Only print every match once
		}
	}
	else
	{
		// Print the counter
		for (int i = 0; i < fieldContents_.size(); i++)
		{
			outStream << '\t' << matched_;
		}
	}
}

}
