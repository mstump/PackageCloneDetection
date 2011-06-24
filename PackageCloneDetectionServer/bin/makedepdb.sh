#!/bin/bash

BINDIR=$(dirname $0)
for i in $1/*.rpm; do
	p=$(basename $i)
	echo "# $p"
	yum deplist $($BINDIR/BasePackage ${p}___)|grep provider:|awk '{print $2}'|cut -d. -f1|sort -u > $p.dep
done
