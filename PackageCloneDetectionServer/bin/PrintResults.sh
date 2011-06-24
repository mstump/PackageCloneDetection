#!/bin/bash

function doit()
{
	while read line; do
		CVEList=$(grep "$line" $1|cut -d\  -f6)
		echo $line $CVEList
	done
}

function doit2()
{
	grep -v ^# $1|grep using|cut -d\  -f1,2,3,4,5|sort -u| doit $1
}

TMPFILE=printResults.tmp.$$
doit2 $1 > $TMPFILE

for i in $(cat $TMPFILE|cut -d\  -f3|sort -u); do
	echo $i
	grep "using $i " $TMPFILE|while read j; do
		echo -e "\t$j"
	done
	echo
done
rm $TMPFILE
