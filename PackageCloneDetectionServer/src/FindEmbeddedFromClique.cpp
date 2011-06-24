#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <list>
#include <cstdio>
#include <cstdlib>
//#include "Kruskal.h"
//#include "Prims.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace boost;
using namespace std;

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
Usage(const char *filename)
{
	fprintf(stderr, "Usage: %s cliqueFile matchesFile weightFile\n", filename);
	exit(1);
}
#if 0
void 
Traverse2(int i, edge *ans, int size, int *n)
{
	for (int k = 0; k < size; k++) {
		if (ans[k].start->name - 1 == i) {
			Traverse2(ans[k].end->name - 1, ans, size, n);
			return;
		}
	}
	*n = i;
}

void 
Traverse1(int i, edge *ans, int size, int *n)
{
	for (int k = 0; k < size; k++) {
		if (ans[k].end->name - 1 == i) {
			Traverse1(ans[k].start->name - 1, ans, size, n);
			return;
		}
	}
	*n = i;
}

void 
Traverse(int i, edge *ans, int size, int *n1, int *n2)
{
	Traverse1(i, ans, size, n1);
	Traverse2(i, ans, size, n2);
}
#endif
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

	while ((ch = getopt(argc, argv, "d")) != EOF) {
		switch (ch) {
		case 'd':
			drawGraph = true;
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

	if (drawGraph) {
		printf("<Scan>\n");
		printf("#<Scan>\n");
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

	stream.open(argv[2]);
	while (!stream.eof()) {
		char s[1024 * 4];

		stream.getline(s, sizeof(s));
		if (s[0] == 0) {
			break;
		} else {
			std::string str(s);
			size_t n1 = str.find_first_of(" ");
			std::string p1 = str.substr(0, n1);
			std::string c = str.substr(n1 + 1);

			weightMap[p1] = atoi(c.c_str());
		}
	}
	stream.close();

	unsigned int count = 0;
	for (	cliqueListIter  = cliqueLists.begin();
		cliqueListIter != cliqueLists.end();
		cliqueListIter++)
	{
		if ((*cliqueListIter)->size() == 0)
			continue;

		std::list<std::string>::iterator iter1, iter2;
		std::string embedded, embedded2, singleEmbedded;
#if 1
		unsigned int N = (*cliqueListIter)->size();
		std::string nameStrings[N];
		typedef boost::adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > graph_t;
		typedef graph_traits<graph_t>::edge_descriptor Edge;
		typedef std::pair<int, int> E;
		std::vector<E> edge_array;
		std::vector<int> edge_weights;

		unsigned int i, j;
		i = 0;
		for (	iter1  = (*cliqueListIter)->begin();
			iter1 != (*cliqueListIter)->end();
			iter1++, i++)
		{
			nameStrings[i] = *iter1;
		}
		i =0;
		for (	iter1  = (*cliqueListIter)->begin();
			iter1 != (*cliqueListIter)->end();
			iter1++, i++)
		{
			j = 0;
			for (	iter2  = (*cliqueListIter)->begin();
				iter2 != (*cliqueListIter)->end();
				iter2++, j++)
			{
				if (j < i) {
					double d;
					int w1, w2, sc;

					sc = scoreMap[std::pair<std::string, std::string>(*iter1, *iter2)];
					w1 = weightMap[*iter1];
					w2 = weightMap[*iter2];
					if (sc > w1 || sc > w2) {
//printf("# ERR\n");
						sc = std::min(w1, w2);
					}
					d = -2 * sc + w1 + w2;
#if 0
					if (w1 == 0 && w2 == 0) {
						d = 0.0;
					} else {
						d = 2.0*(double)sc/(double)(w1 + w2);
					}
					double jj = 1.0 - d/(2.0 - d);
#endif
					edge_array.push_back(E(i, j));
					edge_weights.push_back((int)d);
				}
			}
		}
	
		int *w = new int[edge_weights.size()];
		for (int k = 0; k < edge_weights.size(); k++) {
			w[k] = edge_weights[k];
		}
		graph_t g(edge_array.begin(), edge_array.end(), w, N);
/*
       typedef graph_traits<graph_t>::vertex_descriptor Vertex;
        typedef property_map<graph_t, vertex_index_t>::type IndexMap;
        typedef iterator_property_map<std::vector<Vertex>::iterator, IndexMap> PredMap;

        IndexMap indexMap(get(vertex_index, g));
        graph_traits<graph_t>::vertex_iterator uItr, uEnd;
        j = 0;
        for (tie(uItr, uEnd) = vertices(g); uItr != uEnd; ++uItr, ++j) {
                put(indexMap, *uItr, j);
        }
*/
		std::vector<Edge> spanning_tree;
		std::vector<graph_traits < graph_t >::vertex_descriptor > p(N);

//		kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));
		prim_minimum_spanning_tree(g, &p[0]);

		double myw = 0.0;
		bool yes = false;
		graph_traits<graph_t>::edge_iterator eiter, eiter_end;
/*
		for (boost::tie(eiter, eiter_end) = edges(g); eiter != eiter_end; eiter++) {
			if (find(spanning_tree.begin(), spanning_tree.end(), *eiter) == spanning_tree.end())
				continue;
			std::string i1 = nameStrings[source(*eiter, g)];
			std::string i2 = nameStrings[target(*eiter, g)];
			double f = (double)scoreMap[std::pair<std::string, std::string>(i1, i2)] / (double)weightMap[i1];

			myw += f;
			if (f > 0.6)
				yes = true;
//printf("foo %f\n", f);
		}
*/
		if (drawGraph) {
			int outDegree[N];

			for (int k = 0; k < N; k++)
				outDegree[k] = 0;
			printf("\t<Graph>\n");
			printf("\t\t<Name>%i</Name>\n", count);
			for (int k = 0; k < N; k++) {
				printf("\t\t<Node>%s</Node>\n", nameStrings[k].c_str());
			}
//			for (int k = 0; k < spanning_tree.size(); k++) {
//				s = source(spanning_tree[k], g);
//				d = target(spanning_tree[k], g);
			for (int k = 0; k < N; k++) {
				int s = p[k];
				int d = k;
				if (s == d) continue;
				outDegree[s]++;
				std::string i1 = nameStrings[s];
				std::string i2 = nameStrings[d];
				double f = 1.0;
//				double f = (double)scoreMap[std::pair<std::string, std::string>(i1, i2)] / (double)weightMap[i1];
				printf("\t\t<Edge><src>%s</src><dst>%s</dst><w>%f</w></Edge>\n",
					i1.c_str(), i2.c_str(), f);
			}
			printf("\t</Graph>\n");

			printf("#\t<Graph>\n");
			printf("#\t\t<Name>%i</Name>\n", count);
			for (int k = 0; k < N; k++) {
				printf("#\t\t<Node>\n");
				printf("#\t\t\t<Name>%s</Name>\n", nameStrings[k].c_str());
				printf("#\t\t\t<DegreeCentrality>%f</DegreeCentrality>\n", (double)outDegree[k] / (double)(N - 1));
				printf("#\t\t</Node>\n");
			}
			printf("#\t</Graph>\n");

			count++;
		}
continue;
		myw /= spanning_tree.size();
		if (!yes)
			continue;
		embedded2 = nameStrings[source(spanning_tree[0], g)];
#endif
#if 0
		unsigned int N = (*cliqueListIter)->size();
		prims mst(N, N);
		std::string nameStrings[N];

		unsigned int i = 0, j;
		for (	iter1  = (*cliqueListIter)->begin();
			iter1 != (*cliqueListIter)->end();
			iter1++, i++)
		{
			nameStrings[i] = *iter1;
			j = 0;
			for (	iter2  = (*cliqueListIter)->begin();
				iter2 != (*cliqueListIter)->end();
				iter2++, j++)
			{
				if (*iter1 != *iter2) {
					unsigned int d;

					d = -2*scoreMap[std::pair<std::string, std::string>(*iter1, *iter2)] + weightMap[*iter2] + weightMap[*iter1];
					mst.graph[i][j] = d;
//					printf("(%i,%i)=%i\n", i, j, d);
				}
			}
		}
		std::vector<std::pair<int, int> > ans;
		mst.nodes = N;
		mst.done();
		mst.primsAlgo(ans);
		std::map<int, int> mymap;
		for (int i = 0; i < N; i++)
			mymap[i] = 1;
		for (int i = 0; i < ans.size(); i++) {
			printf("%i -> %i\n", ans[i].first, ans[i].second);
			mymap.erase(ans[i].second);
		}
		printf("mymap %i %i\n", mymap.size(), mymap.begin()->first);
		embedded2 = nameStrings[mymap.begin()->first];
#endif
#if 0
		unsigned int N = (*cliqueListIter)->size();
		edge e[N * (N - 1)];
		mynode n[N];
		std::string nameStrings[N];

		unsigned int i = 0, j, ei = 0;
		for (	iter1  = (*cliqueListIter)->begin();
			iter1 != (*cliqueListIter)->end();
			iter1++, i++)
		{
			n[i].name = i + 1;
			nameStrings[i] = *iter1;
			j = 0;
			for (	iter2  = (*cliqueListIter)->begin();
				iter2 != (*cliqueListIter)->end();
				iter2++, j++)
			{
				if (*iter1 != *iter2) {
					unsigned int d;

					e[ei].start = &n[i];
					e[ei].end = &n[j];
					d = -2*scoreMap[std::pair<std::string, std::string>(*iter1, *iter2)] + weightMap[*iter2] + weightMap[*iter1];
					e[ei].cost = d;
					ei++;
				}
			}
		}
		edge ans[1000];
		int size;
		kruskal(e, N, ei, &size, ans);

		int root = -1;
		int n1, n2, n3, n4;
		Traverse(0, ans, size, &n1, &n2);
		Traverse(1, ans, size, &n3, &n4);
		if (n1 == n3 || n1 == n4) {
			root = n1;
		}
		if (n2 == n3 || n2 == n4) {
			root = n2;
		}
		if (root != -1)
			embedded2 = nameStrings[root];
#if 0
		for (i = 0; i < size; i++) {
printf("%i -> %i\n", ans[i].start->name - 1, ans[i].end->name - 1);
		}
#endif
printf("== %i %i %i %i -- %i\n", n1, n2, n3, n4, root);
fflush(stdout);
#if 0
#endif
		float maxWeight = -1.0;
		unsigned int maxGotmatch = 0;
		float maxSingleWeight = -1.0;

		for (	iter1  = (*cliqueListIter)->begin();
			iter1 != (*cliqueListIter)->end();
			iter1++)
		{
			float weight;
			unsigned int gotmatch;

			gotmatch = 0;
			weight = 0.0;
			for (	iter2  = (*cliqueListIter)->begin();
				iter2 != (*cliqueListIter)->end();
				iter2++)
			{
				double f;

				f = (double)scoreMap[std::pair<std::string, std::string>(*iter1, *iter2)] / (double)weightMap[*iter1];
				if (*iter1 != *iter2) {
					if (scoreMap[std::pair<std::string, std::string>(*iter1, *iter2)] >= 5) {
						gotmatch++;
					}
					if (f > maxSingleWeight) {
						maxSingleWeight = f;
						singleEmbedded = *iter1;
					}
				}
				weight += f;
			}
			if (weight > maxWeight) {
				maxWeight = weight;
				maxGotmatch = gotmatch;
				embedded = *iter1;
			}
		}
		if (maxSingleWeight > 0.5) {
			embedded2 = singleEmbedded;
		} else {
			embedded2 = std::string("unknown_") + singleEmbedded;
			continue;
		}
#if 0
		if (maxGotmatch >= 1 || maxWeight >= (double)(*cliqueListIter)->size() * 0.4) {
			embedded2 = embedded;
		} else {
//			embedded2 = embedded;
			embedded2 = std::string("unknown_") + embedded;
		}
#endif
#endif
		for (	iter1  = (*cliqueListIter)->begin();
			iter1 != (*cliqueListIter)->end();
			iter1++)
		{
			if (*iter1 != embedded) {
				doneMap[embedded2].insert(*iter1);
			}
		}
	}
	if (!drawGraph) {
		std::map<std::string, std::set<std::string> >::iterator dIter1;
		std::set<std::string>::iterator dIter2;
		printf("---BEGIN\n");
		for (	dIter1  = doneMap.begin();
			dIter1 != doneMap.end();
			dIter1++)
		{
			printf("%s\n", BasePackage(dIter1->first).c_str());
			for (	dIter2  = dIter1->second.begin();
				dIter2 != dIter1->second.end();
				dIter2++)
			{
				printf("\t- %s <unknown> (embed)\n", BasePackage(*dIter2).c_str());
			}
			printf("\n");
		}
	} else {
		printf("</Scan>\n");
		printf("#</Scan>\n");
	}
	return 0;
}
