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

	results = soup.find("div", { "class" : "yui-g" } ).findAll("ul")[1:]
	for i in results:
		link = i.find("a").contents[0].strip()
		realLink = i.find("a").get("href")
		words = link.split()
		if words[0] == "[Security":
			package = words[len(words) - 1]
			DownloadUpdate('%s/%s' % (name[0:name.rfind('/')], realLink), package)

def Scrape():
	url= 'http://lists.mandriva.com/security-announce/'
	f=urllib2.urlopen(url)
	data=f.read()

	soup = BeautifulSoup(data)
	results=soup.find("div", { "class" : "yui-g" } ).findAll("li")
	if len(results) == 0:
		return False
	first = True
	for i in results:
		if first:
			first = False
			continue
		names = i.findAll("a")
		for n in names:
			name = n.get("href");
			DownloadMonth('http://lists.mandriva.com/%s' % name)
	return True

Scrape()
