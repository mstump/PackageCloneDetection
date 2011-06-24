#!/bin/bash

function getNames()
{
	pre=1
	cat $1 | egrep -v 'TODO:|NOTE:'|while read line; do
		[ "$line" = "" ] && continue
		if [ "$line" = "---BEGIN" ]; then
			pre=0
			continue
		fi
		if [ $pre -eq 0 ]; then
			echo "$line"|egrep '^-|^\[' > /dev/null
			[ $? -ne 0 ] && echo "$line" |cut -d\  -f1
		fi
	done
}

MERGECVELIST=$1
PACKAGELIST=$2
for lib in $(getNames $3); do
	echo "# checking $lib"
	cveList=$(grep $lib: $MERGECVELIST|cut -d: -f2)
	for cve in $cveList; do
		for app in $(grep ^$lib: $PACKAGELIST|cut -d: -f2|sort -u); do
			echo "# $app:.*$cve"
			grep "$app:.*$cve" $MERGECVELIST > /dev/null || echo "$app using $lib is missing $cve"
		done
	done
done
