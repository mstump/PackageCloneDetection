#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <set>
#include <getopt.h>

#define DEFAULT_DIMALLOC	(1024 * 1024 * 4)
#define DEFAULT_SIMILARITY	(0.6)

static void
Usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s [-u useTheseFeaturesFilename] [-d dimAlloc] [-s similarity] [-r] vecDb ignoreFeaturesFile\n", argv0);
	fprintf(stderr, "\t[-r] sparse\n");
	exit(1);
}

static double
subset_distance_to_x(std::set<unsigned int> *x, std::set<unsigned int> *y, int n, int &matches)
{
	std::set<unsigned int>::iterator iter;
	unsigned int v;

	v = 0;
	for (	iter  = x->begin();
		iter != x->end();
		iter++)
	{
		if (y->find(*iter) != y->end()) {
			v++;
		}
	}
	matches = v;
	return (double)abs(x->size() - v); 
}

static void
LoadFeatureList(const char *filename, std::set<unsigned int> &toIgnore)
{
	std::ifstream toIgnoreStream;
	toIgnoreStream.open(filename);
	if (!toIgnoreStream) {
		fprintf(stderr, "Can't open %s\n", filename);
		exit(1);
	}
	while (!toIgnoreStream.eof()) {
		char q[1024];

		toIgnoreStream.getline(q, sizeof(q));
		std::string str(q);
		std::string indexStr = str.substr(str.find_last_of(' ') + 1);
		int index = atoi(indexStr.c_str());
		toIgnore.insert(index);
	}
	toIgnoreStream.close();
}

int
main(int argc, char *argv[])
{
	std::ifstream stream;
	char *s;
	unsigned int numPackages;
	unsigned int numDim;
	std::set<unsigned int> toIgnore;
	const char *argv0 = argv[0];
	int ch;
	unsigned int dimAlloc = DEFAULT_DIMALLOC;
	float similarity = DEFAULT_SIMILARITY;
	std::vector<std::string> *packageNames;
	std::vector<std::set<unsigned int> *> *packageVectors;
	bool sparse = false;
	bool useTheseFeatures = false;
	std::set<unsigned int> toUseThese;
	bool absolute = false;
	unsigned int matchesThreshold = 3;

	while ((ch = getopt(argc, argv, "m:u:d:s:rx")) != EOF) {
		switch (ch) {
		case 'm':
			matchesThreshold = atoi(optarg);
			break;

		case 'x':
			absolute = true;
			break;

		case 'u':
			useTheseFeatures = true;
			LoadFeatureList(optarg, toUseThese);
			break;

		case 'd':
			dimAlloc = strtoul(optarg, NULL, 10);
			break;

		case 'r':
			sparse = true;
			break;

		case 's':
			similarity = atof(optarg);
			break;

		default:
			Usage(argv0);
		}
	}
	argc -= optind;
	argv += optind;

	// usage: vecDb ignoreList 
	if (argc != 2)
		Usage(argv0);

	LoadFeatureList(argv[1], toIgnore);
	if (useTheseFeatures) {
	}
	char s2[100];
	char s3[4096];
	if (!sparse) {
		s = new char[dimAlloc];
	} else {
		s = NULL;
	}
	stream.open(argv[0]);
	if (!stream)
		exit(1);
	stream.getline(s2, sizeof(s2));
	numPackages = atoi(s2);
	stream.getline(s2, sizeof(s2));
	numDim= atoi(s2);
	for (unsigned int i = 0; i < numDim; i++) {
		stream.getline(s3, sizeof(s3));
	}
	packageNames = new std::vector<std::string>(numPackages);
	packageVectors = new std::vector<std::set<unsigned int> *>(numPackages);
	for (unsigned int i = 0; i < numPackages; i++) {
		std::string s1, packName, vec;
		std::set<unsigned int> *v;

		printf("# %i\n", i);
		fflush(stdout);
		v = new std::set<unsigned int>();
		if (sparse) {
			stream.getline(s3, sizeof(s3));
			(*packageNames)[i] = std::string(s3);
			while (!stream.eof()) {
				stream.getline(s2, sizeof(s2));
				if (s2[0] == '=')
					break;
				unsigned int index = strtoul(s2, NULL, 10);
				if (toIgnore.find(index) == toIgnore.end()) {
					if (!useTheseFeatures ||
						toUseThese.find(index) !=
						toUseThese.end())
					{
						v->insert(index);
					}
				}
			}
		} else {
			stream.getline(s, dimAlloc);
			s1 = std::string(s);
			packName = s1.substr(0, s1.find_first_of('\t'));
			(*packageNames)[i] = packName;
			vec = s1.substr(s1.find_first_of('\t') + 1);
			for (unsigned int j = 0; j < numDim; j++) {
				if (vec.c_str()[j] == '1') {
					if (toIgnore.find(j) == toIgnore.end()) {
						if (!useTheseFeatures ||
							toUseThese.find(j) !=
							toUseThese.end())
						{
							v->insert(j);
						}
					}
				}
			}
		}
		if (v->size() > 3) {
			(*packageVectors)[i] = v;
		} else {
			delete v;
			(*packageVectors)[i] = NULL;
		}
	}
	stream.close();
	for (unsigned int i = 0; i < numPackages; i++) {
		printf("# d %i\n", i);
		fflush(stdout);
		if ((*packageVectors)[i] == NULL)
			continue;
		for (unsigned int j = 0; j < numPackages; j++) {
			if (i != j) {
				double d;
				if ((*packageVectors)[j] == NULL)
					continue;
				int matches;
				d = subset_distance_to_x((*packageVectors)[i], (*packageVectors)[j], numDim, matches);
				if ((absolute == true && matches >= matchesThreshold)) {
					printf("MATCH:%s:%s:%d\n", (*packageNames)[i].c_str(), (*packageNames)[j].c_str(), matches);
				} else if (absolute == false && d < (double)((*packageVectors)[i]->size()) * (1.0 - similarity)) {
					printf("MATCH:%s:%s:%f\n", (*packageNames)[i].c_str(), (*packageNames)[j].c_str(), 1.0 - d / (double)((*packageVectors)[i]->size()));
				}
			}
		}
	}
}
