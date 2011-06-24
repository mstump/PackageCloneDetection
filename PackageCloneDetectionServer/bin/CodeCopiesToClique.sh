#!/bin/bash

first=1
while read line; do
	[ "$line" = "" ] && continue
	echo $line|grep '^[ \t]*-' > /dev/null
	if [ $? -eq 0 ]; then
		echo $line|cut -d\  -f2
	else
		if [ $first -ne 1 ]; then
			echo "="
		else
			first=0
		fi
		echo $line|cut -d\  -f1
	fi
done
