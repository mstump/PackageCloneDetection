#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <set>
#include <map>

#include "biclique.h"

std::map<std::string, std::set<std::string> > g_BipartiteGraph;
std::set<std::string> g_PackageMap;
std::set<std::string> g_FileMap;
std::string filenameDBDirectory = ".";

static void
AddFile(const char *filename)
{
	if (g_PackageMap.find(filename) == g_PackageMap.end()) {
		g_PackageMap.insert(filename);
	}

	std::ifstream stream;
	char thefilename[1024];
	char s[1024];

	snprintf(thefilename, sizeof(thefilename), "%s/%s.files", filenameDBDirectory.c_str(), filename);
	stream.open(thefilename);
	if (!stream)
		return;
	while (!stream.eof()) {
		size_t n;
		stream.getline(s, sizeof(s));

		if (s[0] == 0)
			break;
		std::string q;
		std::string p(s);
		n = p.find_last_of('/');
		if (n == std::string::npos) {
			goto err;
		}
		q = p.substr(n + 1);
		q = q.substr(0, q.size());

		if (g_FileMap.find(q) == g_FileMap.end()) {
			g_FileMap.insert(q);
		}
		g_BipartiteGraph[filename].insert(q);
	}
err:
	stream.close();
}

void
DoBiclique()
{
	std::map<std::string, std::set<std::string> >::iterator iter1;
	SimpleBigraph<std::string, std::string> graph;

	for (	iter1  = g_BipartiteGraph.begin();
		iter1 != g_BipartiteGraph.end();
		iter1++)
	{
		std::set<std::string>::iterator iter2;

		for (	iter2  = iter1->second.begin();
			iter2 != iter1->second.end();
			iter2++)
		{
			graph.add_edge(iter1->first, *iter2);
		}
	}
	std::list<std::pair<std::list<std::string>, std::list<std::string> > > results;
	std::list<std::pair<std::list<std::string>, std::list<std::string> > >::iterator resultsIter;

	graph.get_results(results);
	for (	resultsIter  = results.begin();
		resultsIter != results.end();
		resultsIter++)
	{
		std::list<std::string>::iterator sIter;

		for (	sIter  = resultsIter->first.begin();
			sIter != resultsIter->first.end();
			sIter++)
		{
			printf("%s ", sIter->c_str());
		}
		printf("\n");

		for (	sIter  = resultsIter->second.begin();
			sIter != resultsIter->second.end();
			sIter++)
		{
			printf("%s ", sIter->c_str());
		}
		printf("\n\n");
	}
}


int
main(int argc, char *argv[])
{
	filenameDBDirectory = argv[1];
	for (int i = 2; i < argc; i++) {
		AddFile(argv[i]);
	}
	DoBiclique();
}
