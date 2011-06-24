#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string.h>

static void Usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s filename threshold\n", argv0);
	exit(1);
}

int
main(int argc, char *argv[])
{
	std::ifstream stream;
	unsigned int threshold;

	if (argc != 3) {
		Usage(argv[0]);
	}
	threshold = atoi(argv[2]);
	stream.open(argv[1]);
	if (!stream) {
		exit(1);
	}
	while (!stream.eof()) {
		char s[1024];

		stream.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		if (memcmp(s, "MATCH:", sizeof("MATCH:") - 1) != 0)
			continue;
		std::string str(s);
		size_t n = str.find_last_of(":");
		std::string countStr = str.substr(n + 1);
		unsigned int count = atoi(countStr.c_str());
		if (count >= threshold) {
			printf("%s\n", s);
		}
	}
	stream.close();
	return 0;
}
