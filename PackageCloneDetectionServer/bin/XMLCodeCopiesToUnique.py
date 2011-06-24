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
        print 'Usage: f doc1.xml'
        sys.exit(1)

def sortit(d):
	keys = d.keys()
	keys.sort()
	new_dict = {}
	for k in keys:
		new_dict[k] = d[k]
	return new_dict

done = {}

def loadit(filename):
	doc1 = xml.dom.minidom.parse(filename)
	results = getElements(doc1, [ "EmbeddedPackages", "EmbeddedPackage" ])
	for e in results:
		name = getText(getElements(e, [ "EmbeddedPackageName" ])[0].childNodes)
		packageResults = getElements(e, [ "Package" ])
		if not done.has_key(name):
			done[name] = {}
		for e2 in packageResults:
			name2 = getText(getElements(e2, [ "PackageName" ])[0].childNodes)
			done[name][name2] = 1

if len(sys.argv) < 2:
        Usage()

for filename in sys.argv[1:]:
	loadit(filename)

done = sortit(done)
firstLine = True
for k1,i1 in done.iteritems():
	name = k1
	first = True
	i1 = sortit(i1)
	for k2,i2 in i1.iteritems():
		name2 = k2
		if name != name2:
			if first:
				if not firstLine:
					print
				else:
					firstLine = False
				print '%s' % (name)
				first = False
			print '\t- %s' % (name2)

