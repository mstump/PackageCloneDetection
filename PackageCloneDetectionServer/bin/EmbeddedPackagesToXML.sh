#!/bin/bash

function doit2()
{
	while read line; do
		echo -e "\t\t<Package>"
		echo -e "\t\t\t<PackageName>$line</PackageName>"
		echo -e "\t\t\t<Status>unknown</Status>"
		echo -e "\t\t\t<Sort>unknown_sort</Sort>"
		echo -e "\t\t</Package>"
	done
}

function doit()
{
	while read line; do
		echo -e "\t<EmbeddedPackage>"
		echo -e "\t\t<EmbeddedPackageName>$line</EmbeddedPackageName>"
		grep $line: $1|grep -v $line:$line|cut -d: -f2|sort -u|doit2
		echo -e "\t</EmbeddedPackage>"
	done
}

echo "<EmbeddedPackages>"
cat $1 | cut -d: -f1 |sort -u| doit $1
echo "</EmbeddedPackages>"
