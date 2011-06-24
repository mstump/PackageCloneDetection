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

def processList(clique, dir):
	outputFilename = '/tmp/weka.arff'
	output = open(outputFilename, 'w')
	index = 0
	myFiles = {}
	for p in clique:
		p = p.strip()
		loadPackage(p, dir)
		for f in filenameSet[p]:
			if not myFiles.has_key(f):
				myFiles[f] = 1
			else:
				myFiles[f] += 1
	output.write('@RELATION foo\n')
	myNewFiles = {}
	for k, f in myFiles.iteritems():
		if f != 1:
			myNewFiles[k] = f
	for k, f in myNewFiles.iteritems():
		output.write('@ATTRIBUTE \'%s\' NUMERIC\n' % k)
#	print '@ATTRIBUTE PackageName STRING'
	output.write('@DATA\n')
	for p in clique:
		s = ''
		first = True
		for k, f in myNewFiles.iteritems():
			if first:
				first = False
			else:
				s = '%s,' % s
			if filenameSet[p].has_key(k):
				present = 1
			else:
				present = 0
			s = '%s%i' % (s, present)
#		print '%s,%s' % (s, p)
		output.write('%s\n' % (s))
	output.close()
	input = subprocess.Popen(['/home/silvio/Deakin/MySource/FindVulnEmbeddedPackages/bin/DoWeka.sh', outputFilename], stdout=subprocess.PIPE)
	begin = False
	index = 0
	clusterList = {}
	line = ""
	while True:
		try:
			t = input.communicate()
		except:
			break
		line += t[0].strip()
	for l in line.split("\n"):
		if l == "":
			continue
		if l.upper() == '@DATA':
			begin = True
		elif begin == True:
			str = l[l.rfind(",") + 1:]
			clusterList[index] = str
			index += 1
	clusters = {}
	for k, p in clusterList.iteritems():
		clusters[p] = []
	index = 0
	for p in clique:
		clusters[clusterList[index]].append(p)
		index += 1
	for k, p in clusters.iteritems():
		for l in p:
			print '%s' % l
		print '='
	print '=='

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
