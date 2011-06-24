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
	objcopy -j .rodata $4 ../controlstuff/outfile.1 -O binary
	strings ../controlstuff/outfile.1 > ../controlstuff/outfile.2
	cd $1
	size="$(stat $3/controlstuff/outfile.2 |grep Size:|awk '{print $2}')"
	if [ "$size" -ne 0 ]; then
		cp $3/controlstuff/outfile.2 $2
	fi
	cd $3/mybuildstuff
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

for src_rpm in $sourceDirectory/*.rpm; do
	real_src=$(basename $src_rpm)
	echo "# $real_src"
	if [ "$(ls $targetDirectory/${real_src}__*.files|wc -l)" -eq 0 ]; then
		rm -rf $TMPDIR/mybuildstuff
		rm -rf $TMPDIR/controlstuff
		here=$(pwd)
		mkdir $TMPDIR/controlstuff
		cd $TMPDIR/controlstuff
		mkdir ../mybuildstuff
		cd ../mybuildstuff
		rpm2cpio $src_rpm | cpio -idmv
		pefiles=$(find . -type f -exec file {} \;|grep ELF|cut -d: -f1)
		for j in $pefiles; do
			i=$(basename $j)
			copy_it "$here" "$targetDirectory/${real_src}__${i}.files" "$TMPDIR" $j
		done
		cd $here
		rm -rf $TMPDIR/mybuildstuff
		rm -rf $TMPDIR/controlstuff
	fi
done

rmdir $TMPDIR
