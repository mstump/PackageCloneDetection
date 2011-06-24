#!/bin/bash

# arg1 matchesFile
# arg2 filenameDB

EXEDIR=$(dirname $0)

function doit()
{
	names=$((cat $1|cut -d: -f2; cat $1 |cut -d: -f3)|sort -u)
	echo -n ","
	for n2 in $names; do
		echo -n "$n2,"
	done
	echo
	for n1 in $names; do
		echo -n "$n1,"
		for n2 in $names; do
			line=$(grep ^MATCH:$n1:$n2: $1)
			if [ "$line" = "" ]; then
				s="0.0"
			else
				p1=$(echo $line|cut -d: -f2)
				p2=$(echo $line|cut -d: -f3)
				m=$(echo $line|cut -d: -f4)
				c1=$(wc -l $2/$p1.files|cut -d\  -f1)
				c2=$(wc -l $2/$p2.files|cut -d\  -f1)
				s=$(echo "scale=5;2.0*$m/($c1 + $c2)"|bc)
			fi
			echo -n "$s,"
		done
		echo
	done
}

doit $1 $2
