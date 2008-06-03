/*
 *  logtransformer.h
 *  StatisticsRoom
 *
 *  Created by cbreak on 02.06.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef LOGTRANSFORMER_H
#define LOGTRANSFORMER_H

#include <QList>

#include <memory>

class QTextStream;

namespace Statistics
{
	class Log;
	class Pattern;

/**
This class encapsulates the transformations that produce a tab separated list
from the time stamped log list.
*/
class LogTransformer
{
public:
	LogTransformer();

public: // Statistics&Transformation
	void transformLog(std::tr1::shared_ptr<Log> log, QTextStream & output) const;

public: // Persistency
	/**
	Writes the contained patterns to the passed stream
	 \param out	Stream suitable for writing
	*/
	void writeToStream(QTextStream & out) const;

	/**
	Replaces the current patterns with those loaded from the stream.
	The syntaxi has to be that used by writeToStream()
	 \param in	Stream suitable for reading
	 \return true if the stream was successfully loaded, false if an error occured
	*/
	bool loadFromStream(QTextStream & in);

public: // Pattern handling
	/**
	The start pattern defines the begin of a individual test stimulus.
	When it's matched, the time stamp is stored for later use.
	 \return the start pattern
	*/
	std::tr1::shared_ptr<Pattern> stimulusStart();

	/**
	The end pattern defines the end of an individual test stimulus.
	When it's matched, the time difference to the last start match is calculated
	and the result is printed along with all the other patterns.
	 \return the end pattern
	*/
	std::tr1::shared_ptr<Pattern> stimulusEnd();

	/**
	Data patterns match log entries between start and end.
	Their matches are later printed into the line for the test.
	If a data pattern is not matched, it retains the last matched value.
	 \param idx	The index of the pattern
	 \return the queried pattern, or an invalid pointer if the index is invalid
	*/
	std::tr1::shared_ptr<Pattern> stimulusData(int idx);

	/**
	Inserts a pattern into the data list at the given index.
	 \param idx		The position in which to insert
	 \param pattern	The pattern to insert
	*/
	void insertStimulusData(int idx, std::tr1::shared_ptr<Pattern> pattern);

	/**
	Removes a pattern from the data list at the given index.
	 \param idx	Index of the pattern that is to be removed
	 \return True if the removal was successfull, false if it wasn't
	*/
	bool removeStimulusData(int idx);

	/**
	Returns the number of data patterns.
	 \return the number of data patterns
	*/
	int stimulusDataCount() const;

public: // Used to access both start/end and data patterns as single list
	/**
	Returns the queried pattern.
	Pattern 0 is always stimulusStart, pattern patternCount()-1 is always
	stimulusEnd. The patterns in between are data patterns.
	 \param idx	index of the desired pattern
	 \return pattern with quereied index
	*/
	std::tr1::shared_ptr<Pattern> pattern(int idx);

	/**
	Returns the number of patterns. This is always stimulusDataCount()+2
	 \return number of patterns
	*/
	int patternCount() const;

protected:
	void resetPatterns() const;

private:
	std::tr1::shared_ptr<Pattern> stimulusStart_;
	std::tr1::shared_ptr<Pattern> stimulusEnd_;
	QList<std::tr1::shared_ptr<Pattern> > stimulusPatterns_;
};

}

#endif
