#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <list>
#include <cstdio>
#include <cstdlib>

using namespace std;

static void
Usage(const char *filename)
{
	fprintf(stderr, "Usage: %s cliqueFile matchesFile\n", filename);
	exit(1);
}

int
main(int argc, char *argv[])
{
	std::ifstream stream;
	std::list<std::string> *curList;
	std::list<std::list<std::string> *> cliqueLists;
	std::list<std::list<std::string> *>::iterator cliqueListIter;
	std::map<std::pair<std::string, std::string>, unsigned int> scoreMap;
	std::map<std::string, unsigned int> weightMap;
	const char *argv0 = argv[0];
	std::map<std::string, std::set<std::string> > doneMap;
	bool drawGraph = false;
	int ch;

	while ((ch = getopt(argc, argv, "")) != EOF) {
		switch (ch) {
		default:
			Usage(argv0);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 2) {
		Usage(argv0);
	}

	printf("<Scan>\n");
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
			curList->push_back(std::string(s));
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

			size_t n1 = str.find_first_of(" ");
			std::string p1 = str.substr(0, n1);
			size_t n2 = str.find_first_of(" ", n1 + 1);
			std::string p2 = str.substr(n1 + 1, n2 - n1 - 1);
			size_t n3 = str.find_first_of(" ", n2 + 1);
			std::string c = str.substr(n2 + 1, n3 - n2 - 1);
			unsigned int weight = atoi(c.c_str());

			scoreMap[std::pair<std::string, std::string>(p1, p2)] = weight;
			scoreMap[std::pair<std::string, std::string>(p2, p1)] = weight;
		}
	}
	stream.close();

	unsigned int count = 0;
	for (	cliqueListIter  = cliqueLists.begin();
		cliqueListIter != cliqueLists.end();
		cliqueListIter++, count++)
	{
		std::list<std::string>::iterator iter1, iter2;
		std::string center;
		unsigned int maxWeight = 0;
		unsigned int N = 0;
		unsigned int i, j;
		std::map<unsigned int, std::string> nameStrings;

		i = 0;
		for (	iter1  = (*cliqueListIter)->begin();
			iter1 != (*cliqueListIter)->end();
			iter1++, i++)
		{
			unsigned int weight;
	
			nameStrings[N++] = *iter1;
			weight = 0;
			j = 0;
			for (	iter2  = (*cliqueListIter)->begin();
				iter2 != (*cliqueListIter)->end();
				iter2++, j++)
			{
				unsigned int sc;
				sc = scoreMap[std::pair<std::string, std::string>(*iter1, *iter2)];
				weight += sc;
			}
			if (weight > maxWeight) {
				maxWeight = weight;
				center = *iter1;
			}
		}
	
		printf("\t<Graph>\n");
		printf("\t\t<Name>%i</Name>\n", count);
		for (int k = 0; k < N; k++) {
			printf("\t\t<Node>%s</Node>\n", nameStrings[k].c_str());
		}
		printf("\t\t<Center>%s</Center>\n", center.c_str());
		for (int k = 0; k < N; k++) {
			if (nameStrings[k] != center) {
				unsigned int sc;

				sc = scoreMap[std::pair<std::string, std::string>(center, nameStrings[k])];
				printf("\t\t<Edge><src>%s</src><dst>%s</dst><w>%d</w></Edge>\n",
					center.c_str(), nameStrings[k].c_str(), sc);
			}
		}
		printf("\t</Graph>\n");
	}
	printf("</Scan>\n");
	return 0;
}
