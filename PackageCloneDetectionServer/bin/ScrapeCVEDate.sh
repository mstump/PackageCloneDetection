#!/bin/bash

EXEDIR=$(dirname $0)
for i in $(cat $1|cut -d: -f2-|tr ' ' '\n'|sort -u); do
	d=$($EXEDIR/Download.cve.date.py $i)
	echo $i:$d
done
