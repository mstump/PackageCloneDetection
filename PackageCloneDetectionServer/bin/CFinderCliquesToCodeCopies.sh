#!/bin/bash

grep -v '^#' $1|while read line; do
	files=$(echo $line|cut -d: -f2-)
	for i in $files; do
		echo $i
	done
	echo "="
done
