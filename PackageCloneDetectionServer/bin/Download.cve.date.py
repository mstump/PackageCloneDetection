#!/usr/bin/python

from BeautifulSoup import BeautifulSoup
import urllib2
import sys

#proxy = urllib2.ProxyHandler({ 'http' : 'http://scesare:aP6#wH3!@burnashev.its.deakin.edu.au:3128' })
proxy = urllib2.ProxyHandler({ 'http' : 'http://silvio:!)4l1EgK@burnashev.its.deakin.edu.au:3128' })
opener = urllib2.build_opener(proxy, urllib2.HTTPHandler)
urllib2.install_opener(opener)

def Download(cve):
	try:
		f = urllib2.urlopen('http://web.nvd.nist.gov/view/vuln/detail?vulnId=%s' % cve)
		data=f.read()
		soup = BeautifulSoup(data)
	except:
		print 'error downloading or passing'
		return
	r0 = soup.find("div", "vulnDetail")
	if r0 == None:
		return
	r1 = r0.find("div").contents[1]
	print '%s' % r1

Download(sys.argv[1])
