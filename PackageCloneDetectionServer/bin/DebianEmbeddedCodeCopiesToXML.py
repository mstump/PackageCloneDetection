#!/usr/bin/python

import sys
import os

f = open(sys.argv[1], "r")
start = False
running = False
print '<EmbeddedPackages>'
for line in f:
	line = line.rstrip()
	if start:
		if len(line) != 0:
			if line[0] != '\t' and line[0] != ' ':
				if running:
					print '\t</EmbeddedPackage>'
				running = True
				embeddedPackage = line.split()[0]
				print '\t<EmbeddedPackage>'
				print '\t\t<EmbeddedPackageName>%s</EmbeddedPackageName>' % embeddedPackage
			else:
				if line.find('NOTE:', 0) != -1:
					pass
				elif line.find('TODO:', 0) != -1:
					pass
				else:
					eLine = line.split('-', 1)
					s = eLine[1].strip().split(' ', 2)
					package = s[0]
					if len(s) == 1:
						sort = 'unknown_sort'
						status = 'embed'
					else:
						status = s[1].strip('<>')
						if len(s) > 2:
							sort = s[2]
						else:
							sort = 'unknown_sort'
					print '\t\t<Package>'
					print '\t\t\t<PackageName>%s</PackageName>' % (package)
					print '\t\t\t<Status>%s</Status>' % (status)
					print '\t\t\t<Sort>%s</Sort>' % (sort)
					print '\t\t</Package>'
		
	elif line == '---BEGIN':
		start = True
f.close()
if running:
	print '\t</EmbeddedPackage>'
print '</EmbeddedPackages>'
