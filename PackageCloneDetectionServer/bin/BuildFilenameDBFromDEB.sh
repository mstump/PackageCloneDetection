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
	find . -type f > ../controlstuff/tmpfile.$$
	ssdeep -lr . > ../controlstuff/tmpfile.$$.ssdeep
	cd $1
	cp $3/controlstuff/tmpfile.$$ $2.files
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

for src_rpm in $sourceDirectory/*.tar.gz; do
	real_src=$(basename $src_rpm)
	echo "# $real_src"
	if [ "$(ls $targetDirectory/${real_src}__*.files|wc -l)" -eq 0 ]; then
		rm -rf $TMPDIR/mybuildstuff
		rm -rf $TMPDIR/controlstuff
		here=$(pwd)
		mkdir $TMPDIR/controlstuff
		cp $src_rpm $TMPDIR/controlstuff
		cd $TMPDIR/controlstuff
		mkdir ../mybuildstuff
		name1=$(echo $src_rpm|sed -e 's/.tar.gz$//')
		name1=$(echo $name1|sed -e 's/.orig$//')
		dpkg-source --no-copy -x ${name1}*.dsc ../mybuildstuff/package
		cd ../mybuildstuff
#		tar xzvf ../controlstuff/$real_src
		copy_it "$here" "$targetDirectory/${real_src}__${real_src}" "$TMPDIR"
		cd $here
		rm -rf $TMPDIR/controlstuff
	fi
done

rmdir $TMPDIR
