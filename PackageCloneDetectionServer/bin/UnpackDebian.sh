#!/bin/bash

function Usage()
{
	echo "Usage: $1 [-f] [-r tempDirectory] [-s sourceDirectory] [-t targetDirectory]" > /dev/stderr
	exit 1
}

sourceDirectory=.
targetDirectory=.
forceOutput=0

while getopts ft:s:r: o; do
	case "$o" in
	f)	forceOutput=1;;
	r)	TMPDIR="$OPTARG";;
	s)	sourceDirectory="$OPTARG";;
	t)	targetDirectory="$OPTARG";;
	[?])	Usage $0;;
	esac
done
shift $(expr $OPTIND - 1)

for src_rpm in $sourceDirectory/*.tar.gz; do
	real_src=$(basename $src_rpm)
	echo "# $real_src"
	name1=$(echo $src_rpm|sed -e 's/.tar.gz$//')
	name1=$(echo $name1|sed -e 's/.orig$//')
	echo $name1
	bname=$(basename $name1)
	dpkg-source --no-copy -x ${name1}*.dsc ${targetDirectory}/${bname}
done
