#!/usr/bin/python

from BeautifulSoup import BeautifulSoup
import urllib2
import re
import sys

proxy = urllib2.ProxyHandler({ 'http' : 'http://silvio:!)4l1EgK@burnashev.its.deakin.edu.au:3128' })
opener = urllib2.build_opener(proxy, urllib2.HTTPHandler)
urllib2.install_opener(opener)

def DownloadUpdate(realLink, package):
	cveList = ""
	try:
		f = urllib2.urlopen(realLink)
		data=f.read()
		m = re.findall('CVE-[0-9]+-[0-9]+', data)
		for i in m:
			cveList += '%s ' % i
	except Exception as e:
		print '%s' % str(e)
		pass
	print 'SECURITY:%s:%s' % (package, cveList)

for package in sys.argv[1:]:
	url= 'http://security-tracker.debian.org/tracker/source-package/%s' % package
	DownloadUpdate(url, package)
