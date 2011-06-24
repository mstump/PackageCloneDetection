#!/bin/bash

grep ^MATCH: $1|cut -d: -f2,3,4|tr : ' ' > MatchesForClique.txt
cp /dev/null WeightsForClique.txt
for i in $2/*.files; do
	w=$(wc -l $i|cut -d\  -f1)
	name=$(echo $i|sed -e 's/.*\/\(.*\)\.files$/\1/g')
	echo "$name $w" >> WeightsForClique.txt
done
