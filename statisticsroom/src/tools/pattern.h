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
	Initializes an empty pattern.
	The regular expression is set to "^$" (an empty line).
	The field name is set to "Field Name".
	*/
	Pattern();

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

public: // Accessors
	/**
	Returns the pattern of the regular expression.
	 \return the pattern of the regular expression
	*/
	QString pattern() const;

	/**
	Sets the pattern of the regular expression.
	 \param pattern	A string of a Regular Expression
	*/
	void setPattern(const QString & pattern);

	/**
	Returns the field names as joined string with separator ";".
	 \return the field names as string
	*/
	QString fieldNames() const;

	/**
	Sets the field names with a ";" separated list of names.
	This implicitely also sets the number of fields.
	 \param fieldNames	Field names separated by ";"
	*/
	void setFieldNames(const QString & fieldNames);

public:
	/**
	Matches the string, storing the captures if the match is successfull
	 \return true if the string was matched, false if it wasn't
	*/
	bool match(const QString & string);

	/**
	Resets the stored captures and the match counter.
	Call this before/after using patterns in a transformation.
	*/
	void reset();

	/**
	Prints the headers to the passed output stream
	 \param outStream	A QTextStream open for writing
	*/
	void printHeader(QTextStream & outStream);

	/**
	Prints the Captures to the passed output stream.
	For each header a capture is printed, additional captures are ignored.
	 \param outStream	A QTextStream open for writing
	*/
	void print(QTextStream & outStream);

private:
	int matched_;
	QRegExp regExp_;
	QStringList fieldNames_;
	QStringList fieldContents_;
};

}
