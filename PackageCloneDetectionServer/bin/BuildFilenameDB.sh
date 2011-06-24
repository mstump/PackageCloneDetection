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

function copy_it()
{
	# start in buildstuff dir
	chmod u+rwx -R .
	find . -type f > ../controlstuff/tmpfile.$$.files
        ssdeep -lr . > ../controlstuff/tmpfile.$$.ssdeep
	cd $1
	cp $3/controlstuff/tmpfile.$$.files $2.files
	cp $3/controlstuff/tmpfile.$$.ssdeep $2.ssdeep
	rm -rf $3/mybuildstuff
	cd $3/controlstuff
}

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
	if [ "$(ls $targetDirectory/${real_src}__*.files|wc -l)" -eq 0 ]; then
		rm -rf $TMPDIR/mybuildstuff
		rm -rf $TMPDIR/controlstuff
		here=$(pwd)
		mkdir $TMPDIR/controlstuff
		cd $TMPDIR/controlstuff
		rpm2cpio $src_rpm | cpio -idmv
		bunzip2 *.bz2
		gunzip *.gz
		doit=0
		if [ "$(echo *.tar)" != '*.tar' ]; then
			list="$(echo *.tar)"
			for i in $list; do
				mkdir ../mybuildstuff
				cd ../mybuildstuff
				tar xvf ../controlstuff/$i
				copy_it "$here" "$targetDirectory/${real_src}__${i}" "$TMPDIR"
			done
			doit=1
		fi
		if [ "$(echo *.zip)" != '*.zip' ]; then
			list="$(echo *.zip)"
			for i in $list; do
				mkdir ../mybuildstuff
				cd ../mybuildstuff
				unzip -o ../controlstuff/$i
				copy_it "$here" "$targetDirectory/${real_src}__${i}" "$TMPDIR"
			done
			doit=1
		fi
		if [ $doit -eq 0 ]; then
			echo "# skipping $real_src because it's empty"
		fi
		cd $here
		rm -rf $TMPDIR/controlstuff
#		ssdeep -lr . > /home/silvio/ssdeepresults/${real_src}.ssdeep
	fi
done

rmdir $TMPDIR
