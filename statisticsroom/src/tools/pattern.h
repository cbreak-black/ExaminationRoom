/*
 *  pattern.h
 *  StatisticsRoom
 *
 *  Created by cbreak on 01.06.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <QString>
#include <QRegExp>
#include <QStringList>

class QTextStream;

namespace Statistics
{

class Pattern
{
public:
	/**
	Initializes a new Pattern.
	The regular expression to be matched will be built from the passed string,
	and the field names are built from splitting fieldNames with the separator ";"
	 \param regExp	A string describing a regular expression
	 \param fieldNames	Field Names separated by ";"
	*/
	Pattern(const QString & regExp, const QString & fieldNames);

	/**
	Initializes a new Pattern.
	The regular expression to be matched will be taken as-is,
	and the field names are copied from the passed string list.
	 \param regExp	A regular expression
	 \param fieldNames	Field Names in a QStringList
	 */
	Pattern(const QRegExp & regExp, const QStringList & fieldNames);

public:
	bool match(const QString & string);
	void printHeader(QTextStream & outStream);
	void print(QTextStream & outStream);

private:
	int matched_;
	QRegExp regExp_;
	QStringList fieldNames_;
	QStringList fieldContents_;
};

}
