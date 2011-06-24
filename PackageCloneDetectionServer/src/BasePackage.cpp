#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>

static void Usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s package_name\n", argv0);
	exit(1);
}

int
main(int argc, char *argv[])
{
	std::string str, name1, name2;
	size_t n;
	bool redhat = false, debian = false;
	int ch;
	const char *argv0 = argv[0];

	while ((ch = getopt(argc, argv, "dr")) != EOF) {
		switch (ch) {
		case 'd':
			debian = true;
			break;

		 case 'r':
			redhat = true;
			break;

		default:
			Usage(argv0);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1) {
		Usage(argv0);
	}
  
	str = std::string(argv[0]);
	if (!redhat && !debian) {
		n = str.find("rpm__");
		if (n != std::string::npos) {
			str = str.substr(0, n);
			redhat = true;
		} else {
			n = str.find("tar.gz__");
			if (n != std::string::npos) {
				str = str.substr(0, n);
			}
			debian = true;
		}
	}
	if (redhat) {
		n = str.find_last_of('-');
		if (n == std::string::npos)
			goto err;
		name1 = str.substr(0, n);
		n = name1.find_last_of('-');
		if (n == std::string::npos)
			goto err;
		name2 = name1.substr(0, n);
	} else if (debian) {
		n = str.find_last_of('_');
		if (n == std::string::npos)
			goto err;
		name2 = str.substr(0, n);
	}
	printf("%s\n", name2.c_str());
	return 0;
err:
	printf("%s\n", str.c_str());
	return 1;
}
