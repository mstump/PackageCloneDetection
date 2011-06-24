#!/bin/bash

EXEDIR=$(dirname $0)

function doit()
{
	cat $1 | while read line; do
		packName=$(echo $line|cut -d: -f2)
		cveList="$(grep ^SECURITY:$packName: $1|cut -d: -f3|tr ' ' '\n'|sort -u|tr '\n\r' ' ')"
		echo "$packName:$cveList"
	done
}

doit $1 |sort -u
