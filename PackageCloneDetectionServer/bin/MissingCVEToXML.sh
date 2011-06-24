#!/bin/bash

# erg. so terrible to use native to xml pretty printer

function doit()
{
	while read line; do
		package=$(echo $line|cut -d\  -f3)
		cve=$(echo $line|cut -d\  -f5)
		echo "$package:$cve"
	done
}

packages=$(cat $1 | doit | cut -d: -f1)
echo "<VulnPackages>"
for package in $packages; do
	cveList=$(cat $1 | doit | grep "^$package:" | cut -d: -f2)
	if [ "$cveList" != "" ]; then
		echo -e "\t<Package>"
		echo -e "\t\t<PackageName>$package</PackageName>"
		echo -e "\t\t<CVEList>"
		for cve in $cveList; do
			echo -e "\t\t\t<CVE>$cve</CVE>"
		done
		echo -e "\t\t<CVEList>"
		echo -e "\t</Package>"
	fi
done
echo "</VulnPackages>
