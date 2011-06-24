#!/usr/bin/python

import xml.dom.minidom
import sys

def getElements(root, tagList):
        results = []
        if len(tagList) == 1:
                node = tagList.pop(0)
                for n in root.childNodes:
                        if n.nodeName == node:
                                results.append(n)
        else:
                results = []
                node = tagList.pop(0)
                for n in root.childNodes:
                        if n.nodeName == node:
                                newTagList = []
                                for i in tagList:
                                        newTagList.append(i)
                                r1 = getElements(n, newTagList)
                                for r in r1:
                                        results.append(r)
        return results

def getText(nodeList):
        rc = ""
        for node in nodeList:
                if node.nodeType == node.TEXT_NODE:
                        rc = rc + node.data
        return rc

def Usage():
        print 'Usage: f doc1.xml doc2.xml PackageList'
        sys.exit(1)

def edit_distance(first, second):
        """Find the Levenshtein distance between two strings."""
        if len(first) > len(second):
                first, second = second, first
        if len(second) == 0:
                return len(first)
        first_length = len(first) + 1
        second_length = len(second) + 1
        distance_matrix = [[0] * second_length for x in range(first_length)]
        for i in range(first_length):
                distance_matrix[i][0] = i
        for j in range(second_length):
                distance_matrix[0][j]=j
        for i in xrange(1, first_length):
                for j in range(1, second_length):
                        deletion = distance_matrix[i-1][j] + 1
                        insertion = distance_matrix[i][j-1] + 1
                        substitution = distance_matrix[i-1][j-1]
                        if first[i-1] != second[j-1]:
                                substitution += 1
                        distance_matrix[i][j] = min(insertion, deletion, substitution)
        return distance_matrix[first_length-1][second_length-1]

def mymatch(name1, name2):
	if name1 == name2:
		return True
	if False:
		d = edit_distance(name1, name2)
		s = float(d)/float(max(len(name1), len(name2)))
		if s < 0.3:
			print '# %s matches %s' % (name1, name2)
			return True
	return False

def setSimilarity(set1, set2):
#	return diceCoefficient(set1, set2)
#	return overlapCoefficient(set1, set2)
	return subsetCoefficient(set1, set2)

def subsetCoefficient(set1, set2):
	match = 0
	for k, v in set1.iteritems():
		if set2.has_key(k):
			match += 1
	return float(match)/float(len(set1))


def overlapCoefficient(set1, set2):
	match = 0
	for k, v in set1.iteritems():
		if set2.has_key(k):
			match += 1
	return float(match)/float(min(len(set1), len(set2)))

def diceCoefficient(set1, set2):
	match = 0
	for k, v in set1.iteritems():
		if set2.has_key(k):
			match += 1
	return 2.0*float(match)/float(len(set1) + len(set2))

def FindBestMatch(nameToCheck, set, doc2):
	maxS = 0.0
	bestName = None
	results = getElements(doc2, [ "EmbeddedPackages", "EmbeddedPackage" ])
	for e in results:
		name = getText(getElements(e, [ "EmbeddedPackageName" ])[0].childNodes)
		set2 = {}
		packageResults = getElements(e, [ "Package" ])
		for e2 in packageResults:
			set2[getText(getElements(e2, [ "PackageName" ])[0].childNodes)] = 1
		if set2.has_key(nameToCheck):
			s = setSimilarity(set, set2)
			if s > maxS:
				maxS = s
				bestName = name
#	print '# %s %f %s' % (nameToCheck, maxS, bestName)
	if maxS > 0.0:
		return bestName
	return nameToCheck 

def findit(doc1, doc2, myname1, myname2):
	results = getElements(doc2, [ "EmbeddedPackages", "EmbeddedPackage" ])
	for e in results:
		name = getText(getElements(e, [ "EmbeddedPackageName" ])[0].childNodes)
		if not mymatch(name, myname1):
			continue
		packageResults = getElements(e, [ "Package" ])

		for e2 in packageResults:
			name2 = getText(getElements(e2, [ "PackageName" ])[0].childNodes)
#			print '%s=%s %s=%s' % (name, myname1, name2, myname2)
			if mymatch(name2, myname2):
				return True
		return False
	return False

if len(sys.argv) != 4:
        Usage()

packageList = {}
f = open(sys.argv[3], "r")
for line in f:
	packageList[line.strip()] = 1

doc1 = xml.dom.minidom.parse(sys.argv[1])
doc2 = xml.dom.minidom.parse(sys.argv[2])
gotit = 0
missing = 0
results = getElements(doc1, [ "EmbeddedPackages", "EmbeddedPackage" ])
for e in results:
	name = getText(getElements(e, [ "EmbeddedPackageName" ])[0].childNodes)
	set = {}
	packageResults = getElements(e, [ "Package" ])
	for e2 in packageResults:
		set[getText(getElements(e2, [ "PackageName" ])[0].childNodes)] = 1
	if False:
		if not packageList.has_key(name):
			continue
	if not False:
		bestMatchName = FindBestMatch(name, set, doc2)
	else:
		bestMatchName = name
	if name != bestMatchName:
		print '# upgrading %s to %s' % (name, bestMatchName)
	for e2 in packageResults:
		name2 = getText(getElements(e2, [ "PackageName" ])[0].childNodes)
		sort = getText(getElements(e2, [ "Sort" ])[0].childNodes)
#		if sort != "(embed)":
#			continue
		if False:
			if not packageList.has_key(name2):
				continue
		if (findit(doc1, doc2, bestMatchName, name2) == True or (bestMatchName != name and name == name2)):
			print '# got %s(%s) -> %s' % (bestMatchName, name, name2)
			gotit += 1
		else:
			print '# missing %s(%s) -> %s' % (bestMatchName, name, name2)
			missing += 1
print '# missing=%i gotit=%i' % (missing, gotit)
