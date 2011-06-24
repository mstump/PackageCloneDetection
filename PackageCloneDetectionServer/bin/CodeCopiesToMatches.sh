#!/bin/bash

first=1
e=""
while read line; do
	[ "$line" = "" ] && continue
	[ "$line" = "---BEGIN" ] && continue
	echo $line|grep '^[ \t]*-' > /dev/null
	if [ $? -eq 0 ]; then
		p=$(echo $line|cut -d\  -f2)
		echo MATCH:$e:$p:1
	else
		e=$(echo $line|cut -d\  -f1)
	fi
done
