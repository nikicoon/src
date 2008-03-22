#!/bin/sh
# $Heimdal: import.sh 19739 2007-01-05 21:07:34Z lha $
# $NetBSD: import.sh,v 1.1 2008/03/22 09:42:41 mlelstv Exp $

dir=$1

if test ! -f "$dir"/imdrover.c ; then
    echo $dir doesnt seem to contain imath
    exit 1
fi

rm *.[ch]

headers=`grep ^HDRS "$dir"/Makefile |sed 's/^HDRS=//' | sed 's/imdrover.h//'`
code=`echo $headers | sed 's/imrat.h//g'`
code=`echo $headers | sed 's/rsamath.h//g'`
code=`echo $headers | sed 's/\.h/.c/g'`

for a in $headers $code LICENSE ; do
    cp "$dir"/"$a" .
done

echo  "imathsource =			\\"
for a in $headers $code ; do
    echo "	imath/$a		\\"
done | sort

