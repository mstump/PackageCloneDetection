#!/bin/bash

MERGECVELIST=$1
PACKAGELIST=$2
for lib in $(cat $PACKAGELIST|cut -d: -f1|sort -u); do
	echo "# checking $lib"
	cveList=$(grep $lib: $MERGECVELIST|cut -d: -f2)
	for cve in $cveList; do
		for app in $(grep ^$lib: $PACKAGELIST|cut -d: -f2|sort -u); do
			echo "# $app:.*$cve"
			grep "$app:.*$cve" $MERGECVELIST > /dev/null || echo "$app using $lib is missing $cve"
		done
	done
done
