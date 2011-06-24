#!/usr/bin/python

import sys
import os
import subprocess

#arg1 cliquesfile
#arg2 filenameDir

invertedIndex = {}

def processList(clique, dir):
	grepList = {}
	begin = True
	for k in clique:
		newGrepList = {}
		if begin == True:
			begin = False
			for q, v in invertedIndex[k].iteritems():
				newGrepList[q] = 1
		else:
			for q, v in invertedIndex[k].iteritems():
				if grepList.has_key(q):
					newGrepList[q] = 1
		grepList = newGrepList
	if len(grepList) < 50:
		for g in grepList:
			print '%s' % g
	print '='
	

for p in sys.argv[2:]:
	print '# %s' % p
	f = open(p, 'r')
	for l in f:
		name = os.path.basename(l.strip())
		if not invertedIndex.has_key(name):
			invertedIndex[name] = {}
		invertedIndex[name][p] = 1
	f.close()

f = open(sys.argv[1], "r")
clique = []
for line in f:
	if line.strip() == "=":
		processList(clique, dir)
		clique = []
	else:
		clique.append(line.strip())
processList(clique, dir)
