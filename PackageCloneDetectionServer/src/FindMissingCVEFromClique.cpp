#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <list>
#include <cstdio>
#include <cstdlib>
#include <vector>

static std::string
BasePackage(std::string str)
{
        std::string name1, name2;
        size_t n;
        bool redhat = false, debian = false;

	name2 = str;
        n = str.find("rpm__");
        if (n != std::string::npos) {
                str = str.substr(0, n);
                redhat = true;
        } else {
                n = str.find("tar.gz__");
                if (n != std::string::npos) {
                        str = str.substr(0, n);
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
	return name2;
err:
        printf("%s\n", str.c_str());
	exit(1);
}

static void
GetDate(std::string x, int &d, int &m, int &y)
{
	size_t n1, n2;
	std::string str;

	n1 = x.find_first_of("/");
	str = x.substr(0, n1);
	m = atoi(str.c_str());
	n2 = x.find_first_of("/", n1 + 1);
	str = x.substr(n1 + 1, n2 - n1 - 1);
	d = atoi(str.c_str());
	str = x.substr(n2 + 1);
	y = atoi(str.c_str());
}

static bool
MatchDate(std::string x, std::string y)
{
	int d1, d2, m1, m2, y1, y2;

	if (x.size() == 0 || y.size() == 0) {
		return false;
	}
	GetDate(x, d1, m1, y1);
	GetDate(y, d2, m2, y2);
	if (std::abs((d1 + 31 * m1 + 31 * 12 * y1) - (d2 + 31 * m2 + 31 * 12 * y2)) <= 31 * 3) {
		return true;
	}
	return false;
}

static void
Usage(const char *filename)
{
	fprintf(stderr, "Usage: %s [-c] cliqueFile mergeCVEDB cveDateDB\n", filename);
	fprintf(stderr, "\t[-c] Print cve matches percentage of cliques\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	std::ifstream stream;
	std::list<std::string> *curList;
	std::list<std::list<std::string> *> cliqueLists;
	std::list<std::list<std::string> *>::iterator cliqueListIter;
	const char *argv0 = argv[0];
	std::map<std::string, std::set<std::string> > cveMap;
	std::map<std::string, std::string> cveDateMap;
	bool doMissing = true;
	int ch;

	while ((ch = getopt(argc, argv, "c")) != EOF) {
		switch (ch) {
		case 'c':
			doMissing = false;
			break;

		default:
			Usage(argv0);
		}
	}
	argc -= optind;
	argv += optind;
		
	if (argc != 3) {
		Usage(argv0);
	}
	stream.open(argv[0]);
	curList = new std::list<std::string>();
	while (!stream.eof()) {
		char s[1024];

		stream.getline(s, sizeof(s));
		if (s[0] == '=') {
			curList = new std::list<std::string>();
			cliqueLists.push_back(curList);
		} else if (s[0] == 0) {
			break;
		} else {
			curList->push_back(BasePackage(std::string(s)));
		}
	}
	stream.close();

	stream.open(argv[1]);
	while (!stream.eof()) {
		char s[1024 * 4];

		stream.getline(s, sizeof(s));
		if (s[0] == 0) {
			break;
		} else {
			std::string str(s);

			size_t n1 = str.find_first_of(":");
			std::string p1 = str.substr(0, n1);
			while (1) {
				size_t n2 = str.find_first_of(" ", n1 + 1);
				if (n2 == std::string::npos) {
					break;
				}
				std::string cve = str.substr(n1 + 1, n2 - n1 - 1);
				if (cve.size() == 0) {
					break;
				}
				cveMap[p1].insert(cve);
				n1 = n2;
			}
		}
	}
	stream.close();

	stream.open(argv[2]);
	while (!stream.eof()) {
		char s[1024 * 4];

		stream.getline(s, sizeof(s));
		if (s[0] == 0) {
			break;
		} else {
			std::string str(s);

			size_t n1 = str.find_first_of(":");
			std::string p1 = str.substr(0, n1);
			std::string d = str.substr(n1 + 1);
			cveDateMap[p1] = d;
		}
	}
	stream.close();


	double resInterval = 0.1;
	unsigned int resIntervalN = (int)(1.0 / resInterval) + 1;
	std::vector<unsigned int> resList(resIntervalN);
	unsigned int resCount = 0;

	for (	cliqueListIter  = cliqueLists.begin();
		cliqueListIter != cliqueLists.end();
		cliqueListIter++)
	{
		std::list<std::string>::iterator iter1, iter2;

		for (	iter1  = (*cliqueListIter)->begin();
			iter1 != (*cliqueListIter)->end();
			iter1++)
		{
			std::set<std::string>::iterator cveIter;

			if (cveMap[*iter1].size() > 20)
				continue;
			for (	cveIter  = cveMap[*iter1].begin();
				cveIter != cveMap[*iter1].end();
				cveIter++)
			{
				unsigned int match;
				std::set<std::string> gotCVE;

				match = 1;
				for (	iter2  = (*cliqueListIter)->begin();
					iter2 != (*cliqueListIter)->end();
					iter2++)
				{
					if (*iter1 != *iter2) {
						std::set<std::string>::iterator cveIter2;
						for (	cveIter2  = cveMap[*iter2].begin();
							cveIter2 != cveMap[*iter2].end();
							cveIter2++)
						{
							if (MatchDate(cveDateMap[*cveIter], cveDateMap[*cveIter2]))
							{
								gotCVE.insert(*iter2);
								match++;
								break;
							}
						}
					}
				}
				printf("# %u/%u\n", match, (*cliqueListIter)->size());
				fflush(stdout);
				if (!doMissing) {
					if (match > 7) {
						double s = (double)match/(double)(*cliqueListIter)->size();
						resList[s / resInterval]++;
						resCount++;
					}
				} else {
					if (match > 7) { // && (double)match/(double)(*cliqueListIter)->size() > 0.5) {
						for (	iter2  = (*cliqueListIter)->begin();
							iter2 != (*cliqueListIter)->end();
							iter2++)
						{
							if (*iter1 != *iter2) {
								if (gotCVE.find(*iter2) ==
									gotCVE.end())
								{
									printf("%s using x is missing %s\n", iter2->c_str(), cveIter->c_str());
								}
							}
						}
					}
				}
			}
		}
	}
	if (!doMissing) {
		printf("a,b,\n");
		unsigned int x;
		x = 0;
		for (unsigned int i = 0; i < resIntervalN; i++) {
			x += resList[i];
			printf("%f,%f,\n", (double)i * resInterval, (double)x/(double)resCount);
		}
	}
	return 0;
}
