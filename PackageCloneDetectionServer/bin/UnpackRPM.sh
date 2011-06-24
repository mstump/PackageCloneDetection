#!/bin/bash

TMPDIR="/tmp/.mybad"

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

mkdir $TMPDIR > /dev/null
if [ $? -ne 0 ]; then
	[ $forceOutput -eq 0 ] && { echo "Directory exists: $TMPDIR" 2> /dev/null; exit 1; }
fi

for src_rpm in $sourceDirectory/*.src.rpm; do
	real_src=$(basename $src_rpm)
	echo "# $real_src"

	rm -rf $TMPDIR/mybuildstuff
	rm -rf $TMPDIR/controlstuff
	here=$(pwd)
	mkdir $TMPDIR/controlstuff
	cd $TMPDIR/controlstuff
	chere=$(pwd)
	rpm2cpio $here/$src_rpm | cpio -idmv
	bunzip2 *.bz2
	gunzip *.gz
	doit=0
	if [ "$(echo *.tar)" != '*.tar' ]; then
		list="$(echo *.tar)"
		for i in $list; do
			cd $here
			tar xvf $chere/$i
		done
		doit=1
	fi
	if [ "$(echo *.zip)" != '*.zip' ]; then
		list="$(echo *.zip)"
		for i in $list; do
			cd $here
			unzip -o $chere/$i
		done
		doit=1
	fi
	if [ $doit -eq 0 ]; then
		echo "# skipping $real_src because it's empty"
	fi
	cd $here
	rm -rf $TMPDIR/controlstuff
done

rmdir $TMPDIR
