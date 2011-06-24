#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <map>
#include <getopt.h>

#define DEFAULT_DIMALLOC	(1024 * 1024 * 4)

static void
Usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s [-d dimensionsToAlloc] [-s]\n", argv0);
	fprintf(stderr, "\t[-s] sparse\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	std::ifstream stream;
	char *s = NULL;
	unsigned int numPackages;
	unsigned int numDim;
	std::map<unsigned int, std::string>::iterator nameIter;
	std::map<unsigned int, unsigned int> myFreq;
	std::multimap<unsigned int, unsigned> inverseFreq;
	std::multimap<unsigned int, unsigned>::reverse_iterator inverseFreqIter;
	std::map<unsigned int, std::string> myNames;
	size_t dimAlloc = DEFAULT_DIMALLOC;
	int ch;
	const char *argv0;
	bool sparse = false;

	argv0 = argv[0];
	while ((ch = getopt(argc, argv, "d:s")) != EOF) {
		switch (ch) {
		case 'd':
			dimAlloc = strtoul(optarg, NULL, 10);
			break;

		case 's':
			sparse = true;
			break;

		default:
			Usage(argv0);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1)
		Usage(argv0);

	if (!sparse) {
		s = new char[dimAlloc];
	}
	stream.open(argv[0]);
	if (!stream) {
		fprintf(stderr, "Couldn't open %s\n", argv[0]);
		exit(1);
	}
	char s2[200];
	stream.getline(s2, sizeof(s2));
	numPackages = strtoul(s2, NULL, 10);
	stream.getline(s2, sizeof(s2));
	numDim = strtoul(s2, NULL, 10);
	for (unsigned int i = 0; i < numDim; i++) {
		size_t n;
		char s3[4096];

		stream.getline(s3, sizeof(s3));
		std::string str(s3);
		n = str.find_last_of(' ');
		if (n == std::string::npos)
			goto err;
		std::string name = str.substr(0, n);
		n = str.find_last_of(' ');
		if (n == std::string::npos)
			goto err;
		std::string numStr = str.substr(n + 1);
		myNames[atoi(numStr.c_str())] = name;
	}

	for (unsigned int i = 0; i < numPackages; i++) {
		std::string s1, packName, vec;
		size_t n;

		if (sparse) {
			char s3[4096];

			stream.getline(s3, sizeof(s3));
			packName = std::string(s3);
			while (!stream.eof()) {
				stream.getline(s2, sizeof(s2));
				if (s2[0] == '=')
					break;
				unsigned int index = strtoul(s2, NULL, 10);
				myFreq[index]++;
			}
		} else {
			stream.getline(s, dimAlloc);
			s1 = std::string(s);
			n = s1.find_first_of('\t');
			if (n == std::string::npos)
				goto err;
			packName = s1.substr(0, n);
			n = s1.find_first_of('\t');
			if (n == std::string::npos)
				goto err;
			vec = s1.substr(n + 1);
			for (unsigned int j = 0; j < numDim; j++) {
				if (vec.c_str()[j] == '1') {
					myFreq[j]++;
				}
			}
		}
	}
	for (unsigned int i = 0; i < numDim; i++) {
		inverseFreq.insert(std::pair<unsigned int,unsigned int>(myFreq[i], i));
	}
	for (	inverseFreqIter  = inverseFreq.rbegin();
		inverseFreqIter != inverseFreq.rend();
		inverseFreqIter++)
	{
		int dim = inverseFreqIter->second;
		printf("%s %i %i\n", myNames[dim].c_str(), dim, inverseFreqIter->first);
	}
	return 0;
err:
	fprintf(stderr, "File corrupt\n");
	return 1;
}
