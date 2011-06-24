#!/bin/bash

# arg1 fedora cvelist
# arg2 packagematches
# arg3 debian cvelist

cat $1|while read line; do
	packageName=$(echo $line|cut -d: -f1)
	equiv=$(grep ^Equivalent:.*:$packageName $1|cut -d: -f2)
	if [ "$equiv" = "" ]; then
		grep $packageName: $3 > /dev/null
		if [ $? -eq 0 ]; then
			equiv=$packageName
		else
			echo "# no equivalent $packageName"
			continue
		fi
	fi
	cveList=$(echo $line|cut -d: -f2-)
	for cve in $cveList; do
		for e in $equiv; do
			grep "$e:.*$cve" $3 > /dev/null
			if [ $? -ne 0 ]; then
				echo $packageName in Fedora as $e in Debian is missing CVE $cve
			fi
		done
	done
done
