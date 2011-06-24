#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <list>
#include <set>

void
loadCliques(const char *filename,
	std::list<std::set<std::string> > &cliquesList)
{
	std::ifstream in;
	std::set<std::string> clique;

	in.open(filename);
	while (!in.eof()) {
		char s[1024];

		in.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		if (s[0] == '=') {
			if (clique.size() > 0) {
				cliquesList.push_back(clique);
				clique.clear();
			}
		} else {
			clique.insert(std::string(s));
		}
	}
	in.close();
}

void
loadMatches(const char *filename,
	std::set<std::pair<std::string, std::string> > &matchesTables)
{
	std::ifstream in;

	in.open(filename);
	while (!in.eof()) {
		char s[1024];

		in.getline(s, sizeof(s));
		if (s[0] == 0)
			break;

		std::string str1(s);
		size_t n1 = str1.find_first_of(":");
		std::string str2 = str1.substr(n1 + 1);
		size_t n2 = str2.find_first_of(":");
		std::string match1 = str2.substr(0, n2);
		std::string str3 = str2.substr(n2 + 1);
		size_t n3 = str3.find_first_of(":");
		std::string match2 = str3.substr(0, n3);
	
		if (matchesTables.find(std::pair<std::string, std::string>(match2, match1)) == matchesTables.end())
		{
			matchesTables.insert(std::pair<std::string, std::string>(match1, match2));
		}
//		matchesTables.insert(std::pair<std::string, std::string>(match2, match1));
	}
	in.close();
}

int
main(int argc, char *argv[])
{
	std::set<std::pair<std::string, std::string> > matchesTables;
	std::set<std::pair<std::string, std::string> >::iterator matchesIter;
	std::list<std::set<std::string> > cliquesList;

	loadMatches(argv[1], matchesTables);
	loadCliques(argv[2], cliquesList);

	for (	matchesIter  = matchesTables.begin();
		matchesIter != matchesTables.end();
		matchesIter++)
	{
		std::list<std::set<std::string> >::iterator cliqueIter;

		for (	cliqueIter  = cliquesList.begin();
			cliqueIter != cliquesList.end();
			cliqueIter++)
		{
			if (cliqueIter->find(matchesIter->first) != cliqueIter->end() &&
			
				cliqueIter->find(matchesIter->second) != cliqueIter->end())
			{
				goto doneit;
			}
		}
		printf("%s\n", matchesIter->first.c_str());
		printf("%s\n", matchesIter->second.c_str());
		printf("=\n");
doneit:
		;
	}
}
