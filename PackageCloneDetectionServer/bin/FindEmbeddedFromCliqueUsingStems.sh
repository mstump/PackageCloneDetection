#!/bin/bash

# arg1 = CodeCopies
# arg2 = filenameDir

EXEDIR=$(dirname $0)

function doit()
{
	filenameDir=$1
	shift
	matches=""
	fullnamelist=""
	for j in $*; do
		bj=$($EXEDIR/BasePackage $j)
		fullnamelist="$fullnamelist $bj"
	done
	fullnamelist=$(echo $fullnamelist|tr ' ' '\n'|sort -u)
	((count=0))
	for i in $*; do
		namelist=""
		bi=$($EXEDIR/BasePackage $i)
		for j in $*; do
			bj=$($EXEDIR/BasePackage $j)
			if [ $bi != $bj ]; then
				namelist="$namelist $bj"
			fi
		done
		namelist=$(echo $namelist|tr ' ' '\n'|sort -u)
		m=$($EXEDIR/CheckEmbedded $filenameDir/$i.files $namelist)
		if [ $? -eq 1 ]; then
			q=$(echo $m|cut -d\  -f1)
			matches="$matches $q"
		fi
		((count=count+1))
	done
	m=$($EXEDIR/StemMatches $count $matches)
	if [ $? -eq 1 ]; then
		for i in $fullnamelist; do
			x=$($EXEDIR/BasePackage $i)
			if [ "$x" = "$m" ]; then
				bi=$x
				break;
			fi
		done
		echo $bi
		for i in $fullnamelist; do
			if [ "$bi" != "$i" ]; then
				echo -e "\t- $i"
			fi
		done
		echo
	else
		for i in $*; do
			echo "#$i"
		done
		echo "#="
	fi
}

echo "---BEGIN"
clique=""
cat $1|while read line; do
	if [ "$line" = "=" ]; then
		if [ "$clique" != "" ]; then
			doit $2 $clique
			clique=""
		fi
	else
		clique="$clique $line"
	fi
done
