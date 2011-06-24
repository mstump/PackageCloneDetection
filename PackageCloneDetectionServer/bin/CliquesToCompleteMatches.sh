#!/bin/bash

# arg1 filenameDir
# arg2 cliques_file

function getMatches()
{
	filenameDir=$1
	shift
	files=$(grep -lw $1 $filenameDir/*.files)
	shift
	for i in $*; do
		files=$(grep -wl $i $files)
	done
	for i in $files; do
		echo $(basename $i)|sed -e 's/.files$//g';
	done
}

function BaseNames()
{
	for i in $(cat $1); do
		basename $i
	done
}

function getit()
{
	filenameDir=$1
	shift
	if [ "$*" = "" ]; then
		echo ""
	else
		current=$(BaseNames $filenameDir/$1.files)
		shift
		for i in $*; do
			names=$(BaseNames $filenameDir/$i.files)
			current=$(echo $current $names|tr ' ' '\n'|sort|uniq -d)
		done
		echo $current
	fi
}

function doit()
{
	filenameDir=$1
	shift
	names=$(getMatches $filenameDir $(getit $filenameDir $*))
	num=$(echo $names|wc -w)
	echo "#"
	if [ $num -gt 1 ]; then
		echo $names|tr ' ' '\n'
		echo "="
	fi
}

matches=""
cat $2 | grep -v == | while read line; do
	if [ "$line" = "=" ]; then
		doit $1 $matches
		matches=""
	else
		matches="$matches $line"
	fi
done
doit $1 $matches
