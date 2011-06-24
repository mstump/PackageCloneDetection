#!/bin/bash

TMPFILE=/tmp/rpmpriv.$$
for i in $(cat /tmp/rpmlist); do
	filename=/home/silvio/Distribution_Source2/Distribution_Binaries/Fedora14/$i
	echo "# $i"
	rpmquery -q --dump -p /home/silvio/Distribution_Source2/Distribution_Binaries/Fedora14/$i |grep '^.* .* .* .* ...[246]... .*'|grep -v ' 0000000000000000000000000000000000000000000000000000000000000000 ' > $TMPFILE
        if [ "$(cat $TMPFILE)" != "" ]; then
		cp $TMPFILE /tmp/Priv/${i}___priv
        fi
done
rm $TMPFILE
