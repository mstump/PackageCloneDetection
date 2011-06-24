#!/bin/bash

first=1
e=""
while read line; do
	if [ "$line" = "" -o "$line" = "---BEGIN" ]; then
		echo $line
		continue
	fi
	echo $line|grep ^NOTE > /dev/null
	if [ $? -eq 0 ]; then
		echo -e "\t$line"
		continue
	fi
	echo $line|grep '^[ \t]*-' > /dev/null
	if [ $? -eq 0 ]; then
                [ "$e" = "$p" ] && continue
		p=$(echo $line|cut -d\  -f2)
		grep "^$p:.*CVE" $1 > /dev/null
		if [ $? -eq 0 ]; then
			echo -e "\t$line (security sensitive 2)"
		else
			echo -e "\t$line"
		fi
	else
		e=$(echo $line|cut -d\  -f1)
		grep "^$e:.*CVE" $1 > /dev/null
		if [ $? -eq 0 ]; then
			echo "$line (security sensitive 1)"
		else
			echo "$line"
		fi
	fi
done
