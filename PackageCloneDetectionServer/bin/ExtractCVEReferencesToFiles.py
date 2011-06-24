#!/usr/bin/python

import xml.sax.handler
import xml.dom.minidom
import sys
import re
from string import maketrans

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
        print 'Usage: f allitems.xml'
        sys.exit(1)

def ScanLine(cve, desc):
	itab = "(),/:"
	otab = "     "
	trantab = maketrans(itab, otab)
	desc2 = desc.encode('ascii', 'ignore').translate(trantab)
	words = desc2.split();
	for w in words:
		m = re.search("^.*\.c$", w)
		if m:
			print '%s:%s' % (cve, m.group(0))

def ParseUsingDom(filename):
	doc = xml.dom.minidom.parse(filename)
	results = getElements(doc, [ "cve", "item" ])
	for e in results:
		cve = e.attributes("name")
		desc = getText(getElements(e, [ "desc" ])[0].childNodes)
		ScanLine(cve, desc)

class ResultsHandler(xml.sax.handler.ContentHandler):
	cvename = ""
	inDesc = 0
	desc = ""

	def startElement(self, name, attributes):
		if name == "item":
			self.cveName = attributes.get("name")
			print '# %s' % (self.cveName)
		elif name == "desc":
			self.desc = ""
			self.inDesc = 1

	def characters(self, data):
		if self.inDesc == 1:
			self.desc += data

	def endElement(self, name):
		if name == "desc":
			self.inDesc = 0
			ScanLine(self.cveName, self.desc)

def ParseUsingSax(filename):
	parser = xml.sax.make_parser()
	handler = ResultsHandler()
	parser.setContentHandler(handler)
	parser.parse(filename)

if len(sys.argv) != 2:
        Usage()

ParseUsingSax(sys.argv[1])
