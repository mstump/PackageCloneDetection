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
	fprintf(stderr, "Usage: %s upUntil packageVectors.txt\n", argv0);
	exit(1);
}

int
main(int argc, char *argv[])
{
	std::ifstream stream;
	unsigned int upUntil;

	if (argc != 3)
		Usage(argv[0]);

	upUntil = strtoul(argv[1], NULL, 10);
	stream.open(argv[2]);
	if (!stream) {
		fprintf(stderr, "Couldn't open %s\n", argv[1]);
		exit(1);
	}
	while (!stream.eof()) {
		char s[4096];
		size_t n;

		stream.getline(s, sizeof(s));
		std::string str(s);
		n = str.find_last_of(' ');
		if (n == std::string::npos)
			goto err;
		std::string subStr = str.substr(0, n);
		std::string countStr = str.substr(n + 1);
		unsigned int count = strtoul(countStr.c_str(), NULL, 10);
		if (count <= upUntil)
			break;
		printf("%s\n", subStr.c_str());
	}
	stream.close();
	return 0;
err:
	fprintf(stderr, "File corrupt\n");
	return 1;
}
