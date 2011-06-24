#!/bin/bash

EXEDIR=$(dirname $0)
first=1
e=""
echo "---BEGIN"
while read line; do
	if [ "$line" = "" ]; then
		echo
		continue
	fi
	[ "$line" = "---BEGIN" ] && continue
	echo $line|grep '^[ \t]*-' > /dev/null
	if [ $? -eq 0 ]; then
		[ "$e" = "$p" ] && continue
		echo $e|grep ^unknown_
		if [ $? -eq 0 ]; then
			echo $line
			continue
		fi
		note=""
		sort="(embed)"
		version="<unknown>"
		p=$(echo $line|cut -d\  -f2)
#		list=$(yum deplist $p|grep -w provider|cut -d: -f2|cut -d. -f1|sort -u)
		list=$($EXEDIR/RPMDep.sh $p $e)
		for l in $list; do
			n=$l
#			echo $l $n
			if [ "$e" = "$n" ]; then
				version1="$(yum info $p|grep -w ^Version|tail -n1|cut -d: -f2|awk '{print $1}')"
				release1="$(yum info $p|grep -w ^Release|tail -n1|cut -d: -f2|awk '{print $1}')"
				version="${version1}-${release1}"
				note="NOTE: Might be fixed earlier."
				sort="(embed)"
			fi
		done
		echo -e "\t- $p $version $sort"
		if [ "$note" != "" ]; then
			echo -e "\t$note"
		fi
	else
		e=$(echo $line|cut -d\  -f1)
		echo $e
	fi
done
