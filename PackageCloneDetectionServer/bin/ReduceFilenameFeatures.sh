#!/bin/bash

function doit()
{
	count=0
	while read line; do
		myN=$(echo $line|cut -d\  -f3)
		if [ $myN -le $1 ]; then
			break;
		fi
		echo $line |cut -d\  -f1,2
		((count=count+1))
	done
}

packageCount=$(head -n1 $1)
n=$(echo $packageCount/100|bc)
cat $2 |doit $n
