#!/bin/bash

# arg1 pkglist (from apt-get pkgnames)

for i in $(cat $1); do
	s=$(apt-cache showsrc $i|grep .tar.gz|head -n1|cut -d\  -f4)
	echo $i:$s
done
