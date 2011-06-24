#!/usr/bin/python

from BeautifulSoup import BeautifulSoup
import urllib2
import re

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

def DownloadMonth(name):
	global db

	try:
		f = urllib2.urlopen(name)
		data=f.read()
		soup = BeautifulSoup(data)
	except:
		print 'error downloading or passing'
		return

	results = soup.findAll("ul")[1].findAll("li")
	for i in results:
		link = i.find("a").contents[0].strip()
		realLink = i.find("a").get("href")
		words = link.split()
		if words[0] == "[SECURITY]":
			package = words[len(words) - 1]
			DownloadUpdate('%s/%s' % (name[0:name.rfind('/')], realLink), package)
		else:
			package = words[3]

def Scrape():
	url= 'http://lists.fedoraproject.org/pipermail/package-announce'
	f=urllib2.urlopen(url)
	data=f.read()

	soup = BeautifulSoup(data)
	results=soup.find("table").findAll("tr")
	if len(results) == 0:
		return False
	first = True
	for i in results:
		if first:
			first = False
			continue
		name = i.findAll("a")[3].get("href");
		DownloadMonth('http://lists.fedoraproject.org/pipermail/package-announce/%s' % name)
	return True

Scrape()
