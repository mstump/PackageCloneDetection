#!/bin/bash

for i in $(cat $1); do
	version=$(strings $i|grep '[0-9]*\.[0-9]*\.[0-9]*')
	if [ "$version" != "" ]; then
		echo MATCH:$i:$version
	fi
done
