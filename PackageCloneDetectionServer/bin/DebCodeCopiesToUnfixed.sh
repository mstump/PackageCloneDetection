#!/bin/bash

while read line; do
	echo "$line"|grep -- '- .* (embed)' >/dev/null
	if [ $? -eq 0 ]; then
		echo "$line"|grep '<unknown>' >/dev/null
		if [ $? -eq 0 ]; then
			echo -e "\t$line"
		fi
	else
		echo "$line"|grep 'NOTE: ' > /dev/null|| echo "$line"
	fi
done
