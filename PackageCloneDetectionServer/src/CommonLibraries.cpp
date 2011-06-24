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

	snprintf(thefilename, sizeof(thefilename), "%s/%s.dep", filenameDBDirectory.c_str(), filename);
	stream.open(thefilename);
	if (!stream)
		return;
	while (!stream.eof()) {
		size_t n;
		stream.getline(s, sizeof(s));

		if (s[0] == 0)
			break;
		std::string q(s);

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

void
Print()
{
	std::set<std::string>::iterator iter;
	std::map<std::string, std::set<std::string> >::iterator iter1;

	for (	iter  = g_PackageMap.begin();
		iter != g_PackageMap.end();
		iter++)
	{
		printf("\t\t<Node_A>%s</Node_A>\n", iter->c_str());
	}
	for (	iter  = g_FileMap.begin();
		iter != g_FileMap.end();
		iter++)
	{
		printf("\t\t<Node_B>%s</Node_B>\n", iter->c_str());
	}
	for (	iter1  = g_BipartiteGraph.begin();
		iter1 != g_BipartiteGraph.end();
		iter1++)
	{
		std::set<std::string>::iterator iter2;

		for (	iter2  = iter1->second.begin();
			iter2 != iter1->second.end();
			iter2++)
		{
			printf("\t\t<Edge><src>%s</src><dst>%s</dst></Edge>\n", iter1->first.c_str(), iter2->c_str());
		}
	}

}

int
main(int argc, char *argv[])
{
	filenameDBDirectory = argv[1];
	for (int i = 2; i < argc; i++) {
		std::string p(argv[i]);
		std::string q = p.substr(0, p.find_first_of("__"));
		AddFile(q.c_str());
	}
//	DoBiclique();
	Print();
}
