#!/bin/bash

list=/home/silvio/rpmlist
SAVE=/tmp/resultsscan_rpm_bzip2
for i in $(cat $list); do
	mkdir /home/silvio/rpmroot/rpmroot
	cd /home/silvio/rpmroot/rpmroot
	filename=/home/silvio/RPMS/$i
	echo $i
	rpm2cpio $filename | cpio -idmv
        list="$(find . -type f -exec grep -l '1.0.5, 10-Dec-2007' {} \;)"
        if [ "$list" != "" ]; then
                echo "bzip2:$i" >> $SAVE
                echo $list >> $SAVE
                echo "==" >> $SAVE
        fi
#	grep -ir 'LIBTIFF, Version' . && echo "libtiff $i" >> $SAVE
#	grep -ir 'Mark Adler' . && echo "zlib $i" >> $SAVE
#	grep -ir 'libpng version' . && echo "libpng $i" >> $SAVE
#	grep -ir 'Thomas G' . && echo "libjpeg $i" >> $SAVE
#	grep -ir 'Gershon Elber' . && echo "giflib $i" >> $SAVE
#	grep -ir 'CallWave, Inc.' . && echo "giflib $i" >> $SAVE
	rm -rf /home/silvio/rpmroot/rpmroot
done
