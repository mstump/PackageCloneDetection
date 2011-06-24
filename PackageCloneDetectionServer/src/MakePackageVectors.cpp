#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <set>
#include <vector>
#include <string.h>

static void Usage(const char *argv0)
{
}

static const char DEFAULT_EXTENSION[] = ".files";

int
main(int argc, char *argv[])
{
	std::ifstream stream;
	std::multimap<std::string, std::string> myMap;
	std::map<std::string, unsigned int > nameSet;
	std::map<std::string, unsigned int>::iterator iter;
	unsigned int index;
	int ch;
	const char *argv0 = argv[0];
	const char *extension = DEFAULT_EXTENSION;
	bool sparse = false;
	bool baseStrings = true;

	while ((ch = getopt(argc, argv, "e:sa")) != EOF) {
		switch (ch) {
		case 'a':
			baseStrings = false;
			break;

		case 'e':
			extension = optarg;
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

	if (argc < 1)
		Usage(argv0);

	index = 0;
        for (int i = 0 ; i < argc; i++) {
                std::ifstream stream;
                const char *filename = argv[i];
                char s[1024];

                stream.open(filename);
                if (!stream)
                        continue;
                while (!stream.eof()) {
			size_t n;
                        stream.getline(s, sizeof(s));

			if (s[0] == 0)
				break;
			std::string q;
			if (!baseStrings) {
				q = std::string(s);
			} else {
                       		std::string p(s);
				n = p.find_last_of('/');
				if (n == std::string::npos) {
					goto err;
				}
				q = p.substr(n + 1);
				q = q.substr(0, q.size());
			}

			if (q.size() == 0)
				continue;

			if (nameSet.find(q) == nameSet.end()) {
				nameSet[q] = index++;
			}
		}
		stream.close();
	}
	printf("%i\n", argc - 1);
	printf("%i\n", nameSet.size());
	for (	iter  = nameSet.begin();
		iter != nameSet.end();
		iter++)
	{
		printf("%s %i\n", iter->first.c_str(), iter->second);
	}
        for (int i = 0 ; i < argc; i++) {
                std::ifstream stream;
                const char *filename = argv[i];
		std::vector<unsigned int> *vec = NULL;
		size_t n;
		std::string p2;

                stream.open(filename);
                if (!stream)
                        continue;

                std::string fs(filename);
                std::string p1 = fs.substr(0, fs.size() - strlen(extension));
		n = p1.find_last_of('/');
		if (n == std::string::npos) {
			p2 = p1;
		} else {
			p2 = p1.substr(n + 1);
		}
                fs = std::string(filename);
		unsigned int mag = 0;
		if (!sparse) {
			vec = new std::vector<unsigned int>(nameSet.size());
		} else {
			printf("%s\n", p2.c_str());
		}
		std::set<unsigned int> gotit;
                while (!stream.eof()) {
			char s[1024];

                        stream.getline(s, sizeof(s));

			if (s[0] == 0)
				break;
			std::string q;
			if (!baseStrings) {
				q = std::string(s);
			} else {
        	                std::string p(s);
				n = p.find_last_of('/');
				if (n == std::string::npos) {
					goto err;
				}
				q = p.substr(n + 1);
				q = q.substr(0, q.size());
			}

			if (q.size() == 0)
				continue;

			if (sparse) {
				unsigned int index = nameSet[q];
				if (gotit.find(index) == gotit.end()) {
					printf("%i\n", index);
					gotit.insert(index);
				}
			} else {
				(*vec)[nameSet[q]] = 1;
			}
			mag++;
 		}
		if (sparse) {
			printf("=\n");
		} else {
			printf("%s\t", p2.c_str());
			for (unsigned int j = 0; j < vec->size(); j++) {
				printf("%i", (*vec)[j]);
			}
			printf("\n");
		}
		stream.close();
	}
	return 0;
err:
	fprintf(stderr, "Corrupt\n");
	return 1;
}
