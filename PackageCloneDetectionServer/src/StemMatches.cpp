#include <string.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <map>

int
main(int argc, char *argv[])
{
	std::map<std::string, unsigned int> freqMap;

	if (argc == 2)
		return 0;
	unsigned int count = 0;
	for (unsigned int i = 2; i < argc; i++) {
		freqMap[argv[i]]++;
		count++;
	}
	unsigned int n = freqMap.rbegin()->second;
	double s = (double)n / (double)count;
	if (s >= 0.5 && (double)count / (double)atoi(argv[1]) > 0.33) {
		printf("%s\n", freqMap.rbegin()->first.c_str());
		exit(1);
	}
	return 0;
}
