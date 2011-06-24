#!/bin/bash
#declare -A cache
cache=cache.tmp.$$
function doit()
{
	list=$(yum deplist $1|grep -w provider|cut -d: -f2|cut -d. -f1|sort -u)
	for i in $list; do
		echo $i
		[ "$i" = "$2" ] && exit 0
	done
exit 0
	for i in $list; do
		grep -- ^$i $cache >/dev/null && continue
		echo $i >> $cache
#		[ "${cache[$i]}" = "1" ] && continue
#		cache[$i]=1
		doit $i $2
	done
}
doit $1 $2
rm $cache
