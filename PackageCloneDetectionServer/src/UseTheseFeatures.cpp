#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

int
main(int argc, char *argv[])
{
	std::ifstream in;
	unsigned int limit;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s fileCount limit\n", argv[0]);
		exit(1);
	}
	in.open(argv[1]);
	if (!in) {
		fprintf(stderr, "Can't open %s\n", argv[1]);
		exit(1);
	}
	limit = atoi(argv[2]);
	while (!in.eof()) {
		char s[1024 * 16];
		size_t n;

		in.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		std::string str(s);
		n = str.find_last_of(' ');
		if (n == std::string::npos)
			goto err;
		std::string countstr = str.substr(n + 1);
		unsigned int count = atoi(countstr.c_str());
		if (count > 1 && count < limit) {
			printf("%s\n", str.substr(0, n).c_str());
		}
	}
	in.close();
	return 0;
err:
	printf("Corrupt\n");
	exit(1);
}
