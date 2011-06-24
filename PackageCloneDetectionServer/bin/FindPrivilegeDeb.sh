#!/bin/bash

list=$(find . -name '*.deb')
for i in $list; do
	d=$(ar t $i|grep data)
	ar x $i
	chmod 664 data.*
	if [ $d = "data.tar.gz" ]; then
		gunzip -f data.tar.gz
	elif [ $d = "data.tar.bz2" ]; then
		bunzip2 -f data.tar.bz2
	fi
	tar tvf data.tar|egrep -i '^...s|^......s' > myout.$$
	if [ $? -eq 0 ]; then
		cp myout.$$ ${i}___priv
	fi
	rm data.tar
done
