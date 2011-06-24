#!/bin/bash

list=/home/silvio/deblist
SAVE=/tmp/resultsscan_bzip2_deb
for i in $(cat $list); do
	mkdir /home/silvio/debroot/debroot
	cd /home/silvio/debroot/debroot
	filename=$i
	echo $i
	ar x $filename
	tar xzvf data.tar.gz
	chmod -R u+rwx .
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
	rm -rf /home/silvio/debroot/debroot
done
