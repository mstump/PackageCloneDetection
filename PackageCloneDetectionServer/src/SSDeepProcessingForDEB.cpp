#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <set>
#include <map>
#include <fuzzy.h>

#define FUZZY_THRESHOLD		0

static std::map<std::string,  std::multimap<std::string, std::string> *> g_Cache;
static std::set<std::pair<std::string, std::string> > g_NameCache;

static void
Usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s embeddedMatchesFile filenameDBDir UseTheseFeaturesFile matchesThreshold\n", argv0);
	exit(1);
}

static std::string
basename(std::string p)
{
	size_t n;

	n = p.find_last_of("/");
	return p.substr(n + 1, p.size() - n - 2);
}

static void
makeFeaturesSet(std::set<std::string> &featureSet, const char *filename)
{
	std::ifstream stream;

	stream.open(filename);
	if (!stream) {
		fprintf(stderr, "Can't open %s\n", filename);
		exit(1);
	}
	while (!stream.eof()) {
		char s[1024 * 16];

		stream.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		size_t n = std::string(s).find_last_of(" ");
		featureSet.insert(std::string(s).substr(0, n));
	}
	stream.close();
}

static std::multimap<std::string, std::string>*
loadSigs(std::string name)
{
	std::ifstream stream;
	char s[1024 * 16];
	std::map<std::string,  std::multimap<std::string, std::string> *>::iterator gIter;
	std::multimap<std::string, std::string> *sigs;

	gIter = g_Cache.find(name);
	if (gIter != g_Cache.end()) {
		return gIter->second;
	}
	sigs = new std::multimap<std::string, std::string>();
	g_Cache[name] = sigs;

	stream.open(name.c_str());
	if (!stream) {
		fprintf(stderr, "Can't open %s\n", name.c_str());
		exit(1);
	}
	stream.getline(s, sizeof(s));
	while (!stream.eof()) {
		stream.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		size_t n1, n2, n3;
		n1 = std::string(s).find_first_of(":");
		n3 = std::string(s).find_first_of(":", n1 + 1);
		n2 = std::string(s).find_first_of(",", n1 + 1);
//		std::string sig = std::string(s).substr(n1 + 1, n3 - n1 - 1);
		std::string sig = std::string(s);
		std::string filename = std::string(s).substr(n2 + 1);
		(*sigs).insert(std::pair<std::string, std::string>(basename(filename), sig));
	}
	stream.close();
	return sigs;
}

static void
doCompare(std::string filenameDBDir, std::set<std::string> &featureSet, std::string _p1, std::string _p2, unsigned int matchesThreshold)
{
	std::multimap<std::string, std::string> *s1, *s2;
	std::multimap<std::string, std::string>::iterator s1_iter, s2_iter;
	unsigned int matches;
	std::string p1, p2;
	std::set<std::pair<std::string, std::string> >::iterator nameIter;

	matches = 0;
	p1 = filenameDBDir + "/" + _p1 + ".ssdeep";
	p2 = filenameDBDir + "/" + _p2 + ".ssdeep";

	nameIter = g_NameCache.find(std::pair<std::string, std::string>(p2, p1));
	if (nameIter != g_NameCache.end())
		return;
	g_NameCache.insert(std::pair<std::string, std::string>(p1, p2));

	s1 = loadSigs(p1);
	s2 = loadSigs(p2);
	s2_iter = s2->begin();
	for (	s1_iter  = s1->begin();
		s1_iter != s1->end() && s2_iter != s2->end();
		)
	{
		while (s2_iter != s2->end() && s2_iter->first < s1_iter->first) {
			s2_iter++;
		}
		if (s2_iter == s2->end()) {
			break;
		}
		std::string b1 = s1_iter->first;
		std::string b2 = s2_iter->first;

		if (b1 != b2) {
			s1_iter++;
		} else {
			bool gotit = false;
			if (featureSet.find(b1) != featureSet.end() &&
				featureSet.find(b2) != featureSet.end())
			{
				int similarity = fuzzy_compare(s1_iter->second.c_str(), s2_iter->second.c_str());
				if (similarity > FUZZY_THRESHOLD) {
printf("match %s %s\n", b1.c_str(), b2.c_str());
					matches++;
					gotit = true;
				}
			}
			if (gotit == true) {
				s1_iter++;
				s2_iter++;
			} else {
				std::multimap<std::string, std::string>::iterator s1_next, s2_next;
				s1_next = s1_iter;
				s1_next++;
				s2_next = s2_iter;
				s2_next++;
				if (s1_next == s1->end() && s2_next == s2->end()) {
					s1_iter++;
					s2_iter++;
				} else if (s1_next == s1->end()) {
					s2_iter++;
				} else if (s2_next == s2->end()) {
					s1_iter++;
				} else if (s2_next->first != s2_iter->first) {
					s1_iter++;
				} else {
					s2_iter++;
				}
			}
		}
	}
	if (matches >= matchesThreshold) {
		printf("MATCH:%s:%s:%u\n", _p1.c_str(), _p2.c_str(), matches);
		fflush(stdout);
	}
}

static void
doit(	std::set<std::string> &featureSet,
	const char *embeddedMatchesFile,
	const char *filenameDBDir,
	const char *useTheseFeaturesFile,
	unsigned int matchesThreshold)
{
	std::ifstream stream;

	stream.open(embeddedMatchesFile);
	if (!stream) {
		fprintf(stderr, "Can't open %s\n", embeddedMatchesFile);
		exit(1);
	}
	while (!stream.eof()) {
		char s[1024 * 16];

		stream.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		if (memcmp(s, "MATCH:", sizeof("MATCH:") - 1))
			continue;
		printf("# %s\n", s);
		fflush(stdout);
		size_t n1, n2, n3;
		n1 = std::string(s).find_first_of(":");
		n2 = std::string(s).find_first_of(":", n1 + 1);
		n3 = std::string(s).find_first_of(":", n2 + 1);
		std::string p1 = std::string(s).substr(n1 + 1, n2 - n1 - 1);
		std::string p2 = std::string(s).substr(n2 + 1, n3 - n2 - 1);
		doCompare(filenameDBDir, featureSet, p1, p2, matchesThreshold);
	}
	stream.close();
}

int
main(int argc, char *argv[])
{
	std::set<std::string> featuresSet;
	const char *argv0 = argv[0];

	if (argc != 5) {
		Usage(argv0);
	}
	makeFeaturesSet(featuresSet, argv[3]);
	doit(featuresSet, argv[1], argv[2], argv[3], atoi(argv[4]));
}
