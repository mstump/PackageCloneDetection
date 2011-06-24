#!/bin/bash

# $1 cvedbfilename
# $2 filenameDBDir
# $3 mergecvedb

EXEDIR=$(dirname $0)
for line in $(cat $1|grep -v '^#'|egrep 'CVE-2009|CVE-2010'); do
	cve=$(echo $line|cut -d: -f1)
	packageWithCVE=$(grep $cve $3|cut -d: -f1|head -n1)
	filename=$(echo $line|cut -d: -f2)
	echo 'ssdeep,1.0--blocksize:hash:hash,filename' > match.1
	echo 'ssdeep,1.0--blocksize:hash:hash,filename' > match.2
	echo "# $cve"
	ssdeepCheck=1
#	grep -w "$filename" $2/${packageWithCVE}_*.ssdeep 2> /dev/null >> match.1
	grep -w "$filename" $2/${packageWithCVE}-*.ssdeep 2> /dev/null >> match.1
	if [ $? -ne 0 ]; then
		echo "# Couldn't match $cve $packageWithCVE"
		ssdeepCheck=0
		continue
	fi
	if [ $ssdeepCheck -eq 1 ]; then
		names=$(grep -lw $filename $2/*.files)
		c=$(echo $names|wc -w)
		[ $c -ge 50 ] && continue
		for n in $names; do
			b=$($EXEDIR/BasePackage $(basename $n))
			b1=$(echo $n|sed -e 's/\.files$//g')
			grep "$b:.*$cve" $3 > /dev/null;
			if [ $? -ne 0 ]; then
				grep -w "$filename" $b1.ssdeep |sed -e "s/\".\/package\//$(basename $n)\//g"|sed -e 's/.tar.gz__.*.tar.gz//g' >> match.2
			fi
		done
		matches=$(ssdeep -k match.1 match.2|cut -d: -f2|awk '{print $1}')
	else
		s="NO_SSDEEP"
		matches=$(grep -w $filename $2/*.files)
	fi
	for i in $matches; do
		echo $cve:$filename:$i:$s
	done
done
