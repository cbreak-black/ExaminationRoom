#!/bin/sh
# Parses result logs for correct and incorrect answers
# and prints a crude statistic.
c=`grep Correct $* | wc -l`
i=`grep Incorrect $* | wc -l`
p=`echo "2 k $c $i $c + / 100 * p" | dc`
echo "Correct: "$c", Incorrect: "$i", Success: "$p"%"
