#!/bin/bash

#cd /home/silvio/Debian_Source
list=$(apt-cache pkgnames)
count=0
for i in $list; do
	echo "#$i $count"
	apt-get source $i
	((count=count+1))
done
