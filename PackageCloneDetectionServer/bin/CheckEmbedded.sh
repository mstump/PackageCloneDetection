#!/bin/bash

# arg1 = foo.files
# arg2+ = packageName

function getStem()
{
	echo $1	|sed -e 's/^lib//g' 	\
		|sed -e 's/common//g'	\
		|sed -e 's/compat//g'	\
		|sed -e 's/-//g'	\
		|sed -e 's/\.//g'	\
		|sed -e 's/[0-9]*//g'
}

function getList()
{
	(for i in $(cat $1); do
		dirname -- $i
	done) | sort -u | (while read d; do
		basename -- $d
	done) | sort -u | (while read d; do
		getStem $d
	done) | sort -u
}

fileName=$1
shift
for i in $(getList $fileName); do
	for p in $*; do
		packageName=$(getStem $p)
		if [ "$i" = "$packageName" ]; then
			echo $p match $i
			exit 1
		fi
	done
done
exit 0
