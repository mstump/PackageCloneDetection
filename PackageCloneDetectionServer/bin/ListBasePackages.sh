#!/bin/bash

EXEDIR=$(dirname $0)

function doit()
{
	while read line; do
		name1=$(echo $line|cut -d: -f2)
		name2=$(echo $line|cut -d: -f3)
		echo "$($EXEDIR/BasePackage $name1):$($EXEDIR/BasePackage $name2)"
	done
}

grep ^MATCH: $1 | doit |sort -u
