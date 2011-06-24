#!/usr/bin/python

import sys
import os
import subprocess

#arg1 cliquesfile
#arg2 filenameDir

filenameSet= {}

def loadPackage(package, dir):
	if filenameSet.has_key(package):
		return
	filenameSet[package] = {}
	f = open('%s/%s.files' % (dir, package), "r")
	for line in f:
		line = line.strip()
		filenameSet[package][os.path.basename(line)] = 1

def loadFiles(name, dir):
	mySet = {}
	f = open('%s/%s.files' % (dir, name), 'r')
	for l in f:
		l = os.path.basename(l.strip())
		mySet[l] = 1
	f.close()
	return mySet
	
def processList(clique, dir):
	allFile = {}
	mySet = []
	begin = True
	for p in clique:
		s = loadFiles(p, dir)
		mySet.append(s)
		newAllFile = {}
		if begin == True:
			begin = False
			for q in s:
				newAllFile[q] = 1
		else:
			newAllFile = {}
			for q in s:
				if allFile.has_key(q):
					newAllFile[q] = 1
		allFile = newAllFile
	for k, v in allFile.iteritems():
		print '%s' % k
	print '='
	

dir = sys.argv[2]
f = open(sys.argv[1], "r")
clique = []
for line in f:
	if line.strip() == "=":
		processList(clique, dir)
		clique = []
	else:
		clique.append(line.strip())
processList(clique, dir)
