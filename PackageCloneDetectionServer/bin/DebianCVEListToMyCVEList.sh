#!/bin/bash

cat $1|while read line; do
	[ "$line" = "" ] && continue
	echo $line|grep '^[ \t]*-' > /dev/null
	if [ $? -eq 0 ]; then
		echo SECURITY:$(echo $line|cut -d-  -f2|awk '{print $1}'):$cve
	else
		echo $line|grep '^[ \t]*CVE' > /dev/null
		if [ $? -eq 0 ]; then
			cve=$(echo $line|cut -d\  -f1)
		fi
	fi
done
