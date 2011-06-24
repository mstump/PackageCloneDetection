#!/bin/bash

function Usage()
{
	echo "Usage: $1 [-s targetRPMDirectory] [-d ouputDirectory] [-afghijklmnp12]" > /dev/stderr
	echo "	[-a] Dont download RPM Packages" > /dev/stderr
	echo "	[-f] Force output even if output directory exists" > /dev/stderr
	echo "	[-g] Force Download RPM" > /dev/stderr
	echo "	[-h] Force Filename DB" > /dev/stderr
	echo "	[-i] Force Package Vectors" > /dev/stderr
	echo "	[-j] Force Reduce Filename" > /dev/stderr
	echo "	[-k] Force Reduce Filename2" > /dev/stderr
	echo "	[-p] Force Find Embedded Packages" > /dev/stderr
	echo "	[-l] Force Build Base Packages" > /dev/stderr
	echo "	[-m] Force Scrape CVE" > /dev/stderr
	echo "	[-n] Force Find CVE" > /dev/stderr
	echo "  [-1] For Debian" > /dev/stderr
	echo "  [-2] For Redhat" > /dev/stderr
	exit 1
}

EXEDIR=$(dirname $0)

#OUTPUTDIR="FindVulnEmebeddedPackages_$(date|tr ' ' '_')"
OUTPUTDIR="FindVulnEmebeddedPackages_results"

distro="redhat"
targetRPMDirectory=""
dontDownloadRPMS=0
forceOutput=0
forceDownloadRPM=0
forceFilenameDB=0
forcePackageVectors=0
forceReduceFilename=0
forceReduceFilename2=0
forceBasePackage=0
forceEmbeddedPackages=0
forceScrapeCVE=0
forceFindCVE=0
forceSSDeepPackages=0

while getopts s:d:afghijklmnp12 o; do
		case "$o" in
		s)	targetRPMDirectory="$OPTARG";;
		a)	dontDownloadRPMs=1;;
		d)	OUTPUTDIR="$OPTARG";;
		f)	forceOutput=1;;
		g)	forceDownloadRPM=1;;
		h)	forceFilenameDB=1;;
		i)	forcePackageVectors=1;;
		j)	forceReduceFilename=1;;
		k)	forceReduceFilename2=1;;
		p)	forceEmbeddedPackages=1;;
		l)	forceBasePackage=1;;
		m)	forceScrapeCVE=1;;
		n)	forceFindCVE=1;;
		1)	distro="debian";;
		2)	distro="redhat";;
		[?])	Usage $0;;
		esac
done
shift $(expr $OPTIND - 1)

DEBCODECOPIES=$OUTPUTDIR/DebStyleCodeCopies.txt
DEBCODECOPIES_XML=$OUTPUTDIR/DebStyleCodeCopiesXML.txt
DEBCODECOPIES_XML2=$OUTPUTDIR/DebStyleCodeCopiesXML2.txt
DEBCODECOPIES2=$OUTPUTDIR/DebStyleCodeCopies2.txt
DEBCODECOPIES3=$OUTPUTDIR/DebStyleCodeCopies3.txt
DEBCODECOPIES4=$OUTPUTDIR/DebStyleCodeCopies4.txt
DEBCODECOPIES_UNFIXED=$OUTPUTDIR/DebStyleCodeCopies_unfixed.txt
WEIGHTSFORCLIQUE=$OUTPUTDIR/WeightsForClique.txt
CODECOPIES_2CLIQUE=$OUTPUTDIR/CodeCopies_2clique.txt
CODECOPIES_ALL=$OUTPUTDIR/CodeCopiesAll.txt
CODECOPIES=$OUTPUTDIR/CodeCopies.txt
CODECOPIES2=$OUTPUTDIR/CodeCopies2.txt
CODECOPIES3=$OUTPUTDIR/CodeCopies3.txt
CODECOPIES4=$OUTPUTDIR/CodeCopies4.txt
MATCHESFORCLIQUE=$OUTPUTDIR/MatchesForClique.txt
CVEDB=$OUTPUTDIR/CVEDatabase.txt
CVEDB2=$OUTPUTDIR/CVEDatabase2.txt
MERGEDCVEDB=$OUTPUTDIR/MergedCVEDatabase.txt
EMBEDDEDPACKAGESDB=$OUTPUTDIR/EmbeddedPackagesDatabase.xml
EMBEDDEDPACKAGESDB2=$OUTPUTDIR/EmbeddedPackagesDatabase_abs.xml
EMBEDDEDPACKAGESDB3=$OUTPUTDIR/EmbeddedPackagesDatabase_abs2.xml
EMBEDDEDPACKAGESDB4=$OUTPUTDIR/EmbeddedPackagesDatabase_abs3.xml
EMBEDDEDPACKAGESDB_MATCH=$OUTPUTDIR/EmbeddedPackagesDatabase_match.xml
PACKAGEVECTORS=$OUTPUTDIR/PackageVectors.txt
IGNOREFILENAMES=$OUTPUTDIR/FilenamesToIgnore.txt
FILENAMECOUNT=$OUTPUTDIR/FilenameCount.txt
BASEPACKAGEDB=$OUTPUTDIR/BasePackageDB.txt
BASEPACKAGEDB2=$OUTPUTDIR/BasePackageDB2.txt
RPMTIMESTAMPFILE=$OUTPUTDIR/DownloadRPM_Date.txt
USETHESEFEATURES=$OUTPUTDIR/UseTheseFeatures.txt
RESULTS1=$OUTPUTDIR/Results1.txt

DOWNLOADRPM_DONE=$OUTPUTDIR/.downloadrpm_done
FILENAMEDB_DONE=$OUTPUTDIR/.filenamedb_done
PACKAGEVECTORS_DONE=$OUTPUTDIR/.packagevectors_done
REDUCEFILENAME_DONE=$OUTPUTDIR/.reducefilename_done
REDUCEFILENAME2_DONE=$OUTPUTDIR/.reducefilename2_done
SSDEEP_DONE=$OUTPUTDIR/.ssdeep_done
BASEPACKAGE_DONE=$OUTPUTDIR/.basepackage_done
SCRAPECVE_DONE=$OUTPUTDIR/.scrapecve_done
FINDCVE_DONE=$OUTPUTDIR/.findcve_done
EMBEDDEDPACKAGES_DONE=$OUTPUTDIR/.embeddedpackages_done

mkdir $OUTPUTDIR 2> /dev/null
[ $? -ne 0 -a $forceOutput -eq 0 ] && { echo "Output directory exists. Need to use -f." > /dev/stderr; exit 1; }

echo "# Using output directory $OUTPUTDIR" > /dev/stderr
if [  $dontDownloadRPMS -eq 0 ]; then
	if [ ! -f "$DOWNLOADRPM_DONE" -o $forceDownloadRPM -eq 1 ]; then
		echo -n "# Downloading RPMS..." > /dev/stderr
		if [ "$targetRPMDirectory" != "" ]; then
			$EXEDIR/DownloadAllSourceRPM.sh -d $targetRPMDirectory
			[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
		else
			targetRPMDirectory=$OUTPUTDIR/RPMS
			mkdir $targetRPMDirectory > /dev/null
			$EXEDIR/DownloadAllSourceRPM.sh -d $targetRPMDirectory
			[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
		fi
		date > $RPMTIMESTAMPFILE
		rm $FILENAMEDB_DONE
		touch $DOWNLOADRPM_DONE
		echo "Done" > /dev/stderr
	fi
else
	if [ "$targetRPMDirectory" = "" ]; then
		targetRPMDirectory=$OUTPUTDIR/RPMS
	fi
fi

if [ ! -f "$FILENAMEDB_DONE" -o $forceFilenameDB -eq 1 ]; then
	echo -n "# Building FileName Database..." > /dev/stderr
	mkdir $OUTPUTDIR/filenameDB
	mkdir $OUTPUTDIR/RPMTemp
	if [ "$distro" = "debian" ]; then
		$EXEDIR/BuildFilenameDBFromDEB.sh -f -r $OUTPUTDIR/RPMTemp -s $targetRPMDirectory -t $OUTPUTDIR/filenameDB
	elif [ "$distro" = "redhat" ]; then
		$EXEDIR/BuildFilenameDB.sh -f -r $OUTPUTDIR/RPMTemp -s $targetRPMDirectory -t $OUTPUTDIR/filenameDB
	else
		exit
	fi
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
	touch $FILENAMEDB_DONE
	echo "Done" > /dev/stderr
	rm $PACKAGEVECTORS_DONE
fi

if [ ! -f "$PACKAGEVECTORS_DONE" -o $forcePackageVectors -eq 1 ]; then
	echo -n "# Making Package Vectors..." > /dev/stderr
	$EXEDIR/MakePackageVectors -s $OUTPUTDIR/filenameDB/*.files > $PACKAGEVECTORS
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
	touch $PACKAGEVECTORS_DONE
	echo "Done" > /dev/stderr
	rm $REDUCEFILENAME_DONE
fi

if [ ! -f "$REDUCEFILENAME_DONE" -o $forceReduceFilename -eq 1 ]; then
	echo -n "# Reducing Features..." > /dev/stderr
	$EXEDIR/CountDuplicateFilenames -s $PACKAGEVECTORS > $FILENAMECOUNT
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
	touch $REDUCEFILENAME_DONE
	echo "Done" > /dev/stderr
	rm $REDUCEFILENAME2_DONE
fi

if [ ! -f "$REDUCEFILENAME2_DONE" -o $forceReduceFilename2 -eq 1 ]; then
	echo -n "# Reducing Features2..." > /dev/stderr
	packageCount=$(head -n1 $PACKAGEVECTORS)
	reduceNum=$(echo $packageCount/100|bc)
	$EXEDIR/ReduceFilenameFeatures $reduceNum $FILENAMECOUNT > $IGNOREFILENAMES
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
# 
#	$EXEDIR/GetSourceFeatures.sh $FILENAMECOUNT > $USETHESEFEATURES
	$EXEDIR/UseTheseFeatures $FILENAMECOUNT 50 > $USETHESEFEATURES
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
#
	touch $REDUCEFILENAME2_DONE
	echo "Done" > /dev/stderr
	rm $EMBEDDEDPACKAGES_DONE
fi

if [ ! -f "$EMBEDDEDPACKAGES_DONE" -o $forceEmbeddedPackages -eq 1 ]; then
	echo -n "# Finding Embedded Packages..." > /dev/stderr
	$EXEDIR/FindEmbeddedPackages -u $USETHESEFEATURES -r $PACKAGEVECTORS $IGNOREFILENAMES > $EMBEDDEDPACKAGESDB
	$EXEDIR/FindEmbeddedPackages -m 1 -x -u $USETHESEFEATURES -r $PACKAGEVECTORS $IGNOREFILENAMES > $EMBEDDEDPACKAGESDB2
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
	echo "Done" > /dev/stderr
	touch $EMBEDDEDPACKAGES_DONE
	rm $SSDEEP_DONE
fi

if [ ! -f "$SSDEEP_DONE" -o $forceSSDeepPackages -eq 1 ]; then
	echo -n "# Culling Embedded Packages Using SSDeep..." > /dev/stderr
	export LD_LIBRARY_PATH=/usr/local/lib
	$EXEDIR/SSDeepProcessingForDEB $EMBEDDEDPACKAGESDB2 $OUTPUTDIR/filenameDB/ $USETHESEFEATURES 1 > $EMBEDDEDPACKAGESDB3
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
	echo "Done" > /dev/stderr
	touch $SSDEEP_DONE
	rm $BASEPACKAGE_DONE
fi

if [ ! /bin/true ]; then
	$EXEDIR/CullMatchesByCount $EMBEDDEDPACKAGESDB3 2 > $EMBEDDEDPACKAGESDB4
fi

if [ ! /bin/true ]; then
	echo -n "# Finding Cliques..." > /dev/stderr
	rm -rf ${MATCHESFORCLIQUE}_files
	grep ^MATCH: $EMBEDDEDPACKAGESDB4|cut -d: -f2,3,4|tr ':' ' ' > $MATCHESFORCLIQUE
	/home/silvio/Deakin/CFinder-2.0.4--1437/CFinder_commandline -i $MATCHESFORCLIQUE -l /home/silvio/Deakin/CFinder-2.0.4--1437/licence.txt -t 0.0001
	echo "Done" > /dev/stderr
fi

if [ /bin/true ]; then
	cp /dev/null $WEIGHTSFORCLIQUE
	$EXEDIR/CFinderCliquesToCodeCopies.sh ${MATCHESFORCLIQUE}_files/cliques > $CODECOPIES
	grep -v '=' $CODECOPIES|sort -u|while read line; do
		c=$(wc -l ${OUTPUTDIR}/filenameDB/${line}.files|cut -d\  -f1)
		echo $line $c >> $WEIGHTSFORCLIQUE
	done
fi

if [ /bin/true ]; then
	$EXEDIR/Find2Cliques $EMBEDDEDPACKAGESDB4 $CODECOPIES > $CODECOPIES_2CLIQUE
	cat $CODECOPIES $CODECOPIES_2CLIQUE > $CODECOPIES_ALL
fi

if [ /bin/true ]; then
	$EXEDIR/FindEmbeddedFromCliqueUsingStems.sh $CODECOPIES_ALL ${OUTPUTDIR}/filenameDB > $CODECOPIES2
fi

if [ /bin/true ]; then
	grep -v '^#' $CODECOPIES2 > $CODECOPIES3
	echo "=" > $CODECOPIES4
	grep '^#' $CODECOPIES2 |cut -b2- >> $CODECOPIES4
fi

if [ /bin/true ]; then
	$EXEDIR/FindEmbeddedFromClique $CODECOPIES4 $MATCHESFORCLIQUE $WEIGHTSFORCLIQUE > $DEBCODECOPIES
fi

if [ /bin/true ]; then
	$EXEDIR/DebianEmbeddedCodeCopiesToXML.py $DEBCODECOPIES > $DEBCODECOPIES_XML
	$EXEDIR/DebianEmbeddedCodeCopiesToXML.py $CODECOPIES3 > $DEBCODECOPIES_XML2
	$EXEDIR/XMLCodeCopiesToUnique.py $DEBCODECOPIES_XML $DEBCODECOPIES_XML2 > $DEBCODECOPIES2
fi

if [ ! /bin/true ]; then
	cat $DEBCODECOPIES2 | $EXEDIR/CodeCopiesToFixed.sh > $DEBCODECOPIES3
#	cat $DEBCODECOPIES3 | $EXEDIR/CodeCopiesToSecurityCodeCopies.sh $MERGEDCVEDB > $DEBCODECOPIES4
fi

if [ ! /bin/true ]; then
	cat $DEBCODECOPIES3 | $EXEDIR/DebCodeCopiesToUnfixed.sh > $DEBCODECOPIES_UNFIXED
	cat $DEBCODECOPIES_UNFIXED | $EXEDIR/CodeCopiesToMatches.sh > $EMBEDDEDPACKAGESDB_MATCH
fi

if [ ! -f "$BASEPACKAGE_DONE" -o $forceBasePackage -eq 1 ]; then
	echo -n "# Building Base Packages..." > /dev/stderr
	grep ^MATCH: $EMBEDDEDPACKAGESDB_MATCH > $BASEPACKAGEDB
	$EXEDIR/ListBasePackages.sh $BASEPACKAGEDB |grep -v '^\(.*\):\1$' > $BASEPACKAGEDB2
	touch $BASEPACKAGE_DONE
	echo "Done" > /dev/stderr
	rm $FINDCVE_DONE
fi

if [ ! -f "$SCRAPECVE_DONE" -o $forceScrapeCVE -eq 1 ]; then
	echo -n "# Scraping CVEs..." > /dev/stderr
	if [ "$distro" = "debian" ]; then
		$EXEDIR/ScrapeCVEDebianTracker.py $(cat $BASEPACKAGEDB2|awk -F: '{print $1 "\n" $2}'|sort -u) > $CVEDB
	elif [ "$distro" = "redhat" ]; then
		$EXEDIR/ScrapeCVE.py > $CVEDB
	else
		exit 1
	fi
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
	touch $SCRAPECVE_DONE
	echo "Done" > /dev/stderr
	rm $FINDCVE_DONE
fi

if [ ! -f "$FINDCVE_DONE" -o $forceFindCVE -eq 1 ]; then
	if [ "$distro" = "redhat" ]; then
		basePackageOpt="-r"
	elif [ "$distro" = "debian" ]; then
		basePackageOpt="-d"
	else
		exit 1
	fi
	echo -n "# Finding Missing CVEs..." > /dev/stderr
	(cat $CVEDB | while read line; do
		n1=$(echo $line|cut -d: -f1)
		n2=$(echo $line|cut -d: -f2)
		n3=$(echo $line|cut -d: -f3)
		echo "$n1:$($EXEDIR/BasePackage $basePackageOpt $n2):$n3"
	done) > $CVEDB2
	$EXEDIR/MergeCVE.sh $CVEDB2 > $MERGEDCVEDB
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
	$EXEDIR/FindMissingCVE.sh $MERGEDCVEDB $BASEPACKAGEDB2 > $RESULTS1
	[ $? -ne 0 ] && { echo "Error" 2> /dev/stderr; exit 1; }
	touch $FINDCVE_DONE
	echo "Done" > /dev/stderr
fi

$EXEDIR/PrintResults.sh $RESULTS1
echo "# Analysis Complete!!! " > /dev/stderr
