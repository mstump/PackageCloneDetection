#!/bin/bash

function Usage()
{
	echo "Usage: $1 [-d targetDirectory]" > /dev/stderr
	exit 1
}

while getopts d: o; do
	case "$o" in
	d)	targetDirectory="$OPTARG";;
	[?])	Usage $0;;
	esac
done
shift $(expr $OPTIND - 1)

if [ "$targetDirectory" != "" ]; then
	cd $targetDirectory
fi

package_list=$(yum list |tail -n +2|awk '{print $1}')
for package in $package_list; do
	yumdownloader $package
done
