#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <set>
#include <list>
#include <algorithm>
#include <map>
#include <scotch.h>

#define PARTITION_SIZE	(25)

class MyVertex {
public:
	std::set<MyVertex *> m_Neighbours;
	bool m_Visited;
};

class MyStringVertex : public MyVertex {
public:
	MyStringVertex(std::string x) : m_String(x) {}

	std::string m_String;
};

class MyGraph {
public:
	void addEdge(MyVertex *x, MyVertex *y);
	void getComponents(std::list<MyGraph *> &components);
	void depthFirstVisit(MyVertex *v, MyGraph *g);
	void partitionByOutdegree(unsigned int vn);
	void partition(unsigned int k, std::list<MyGraph *> &partitions);
	unsigned int numEdges() const;
	void output(std::ofstream &stream,
		std::map<MyVertex *, unsigned int> &indexByVertex,
		std::map<unsigned int, MyVertex *> vertexByIndex);

	void setVerticiesVisited(bool state);

	std::set<MyVertex *> m_Verticies;
//	std::list<std::pair<MyVertex *, MyVertex *> > m_Edges;
};

unsigned int
MyGraph::numEdges() const
{
	unsigned int e;
	std::set<MyVertex *>::iterator iter;

	e = 0;
	for (	iter  = m_Verticies.begin();
		iter != m_Verticies.end();
		iter++)
	{
		e += (*iter)->m_Neighbours.size();
	}
	return e / 2;
}

void
MyGraph::output(std::ofstream &stream,
	std::map<MyVertex *, unsigned int> &indexByVertex,
	std::map<unsigned int, MyVertex *> vertexByIndex)
{
	std::set<MyVertex *>::iterator iter, iter2;
	stream << "0" << std::endl;
	stream << m_Verticies.size() << "\t" << numEdges() * 2 << std::endl;
	stream << "0\t000" << std::endl;
	for (	iter  = m_Verticies.begin();
		iter != m_Verticies.end();
		iter++)
	{
		stream << (*iter)->m_Neighbours.size();
		for (	iter2  = (*iter)->m_Neighbours.begin();
			iter2 != (*iter)->m_Neighbours.end();
			iter2++)
		{
			stream << "\t" << indexByVertex[*iter2];
		}
		stream << std::endl;
	}
}

void
MyGraph::partition(unsigned int k, std::list<MyGraph *> &partitions)
{
	SCOTCH_Graph grafdat;
	std::ofstream stream;
	std::map<MyVertex *, unsigned int> indexByVertex;
	std::map<unsigned int, MyVertex *> vertexByIndex;
	SCOTCH_Strat strat;
	std::set<MyVertex *>::iterator iter;

	unsigned int i;
	for (	iter  = m_Verticies.begin(), i = 0;
		iter != m_Verticies.end();
		iter++, i++)
	{
		indexByVertex[*iter] = i;
		vertexByIndex[i] = *iter;
	}

	stream.open("t.grf");
	output(stream, indexByVertex, vertexByIndex);
	stream.close();

	FILE *f;
	f = fopen("t.grf", "r");
	if (SCOTCH_graphInit(&grafdat) != 0) {
		fprintf(stderr, "graphInit\n");
		exit(1);
	}
	SCOTCH_graphLoad(&grafdat, f, -1, 0);
	fclose(f);
	SCOTCH_stratInit(&strat);
	SCOTCH_Num results[m_Verticies.size()];
	SCOTCH_graphPart(&grafdat, k, &strat, results);
	SCOTCH_stratExit(&strat);
	SCOTCH_graphExit(&grafdat);

	typedef MyGraph *MyGraphPtr;
	MyGraphPtr r[k];
	for (unsigned int i = 0; i < k; i++) {
		r[i] = new MyGraph();
		partitions.push_back(r[i]);
	}
	for (unsigned int i = 0; i < m_Verticies.size(); i++) {
		for (	iter  = vertexByIndex[i]->m_Neighbours.begin();
			iter != vertexByIndex[i]->m_Neighbours.end();
			iter++)
		{
			if (results[i] == results[indexByVertex[*iter]]) {
				r[results[i]]->addEdge(vertexByIndex[i], *iter);
			}
		}
	}
}

void
MyGraph::partitionByOutdegree(unsigned int vn)
{
	std::set<MyVertex *>::iterator iter;

	for (	iter  = m_Verticies.begin();
		iter != m_Verticies.end();
		)
	{
		std::set<MyVertex *>::iterator next = iter;
		next++;
		if ((*iter)->m_Neighbours.size() >= vn) {
			std::set<MyVertex *>::iterator iter2;
			for (	iter2  = m_Verticies.begin();
				iter2 != m_Verticies.end();
				iter2++)
			{
				(*iter2)->m_Neighbours.erase(*iter);
			}
			m_Verticies.erase(iter);
		}
		iter = next;
	}
}

void
MyGraph::setVerticiesVisited(bool state)
{
	std::set<MyVertex *>::iterator iter;

	for (	iter  = m_Verticies.begin();
		iter != m_Verticies.end();
		iter++)
	{
		(*iter)->m_Visited = state;
	}
}

void
MyGraph::depthFirstVisit(MyVertex *v, MyGraph *g)
{
	std::set<MyVertex *>::iterator iter;

	if (v->m_Visited)
		return;
	v->m_Visited = true;
	for (	iter  = v->m_Neighbours.begin();
		iter != v->m_Neighbours.end();
		iter++)
	{
		g->addEdge(v, *iter);
		depthFirstVisit(*iter, g);
	}
}

void
MyGraph::getComponents(std::list<MyGraph *> &components)
{
	std::set<MyVertex *>::iterator iter;

	setVerticiesVisited(false);
	for (	iter  = m_Verticies.begin();
		iter != m_Verticies.end();
		iter++)
	{
		if ((*iter)->m_Visited == false) {
			MyGraph *newGraph = new MyGraph();
			depthFirstVisit(*iter, newGraph);
			if (newGraph->m_Verticies.size() > 0) {
				components.push_back(newGraph);
			} else {
				delete newGraph;
			}
		}
	}
}

void
MyGraph::addEdge(MyVertex *x, MyVertex *y)
{
	std::set<MyVertex *>::iterator iter;

	iter = m_Verticies.find(x);
	if (iter == m_Verticies.end()) {
		m_Verticies.insert(x);
	}
	iter = m_Verticies.find(y);
	if (iter == m_Verticies.end()) {
		m_Verticies.insert(y);
	}
	x->m_Neighbours.insert(y);
	y->m_Neighbours.insert(x);
//	m_Edges.push_back(std::pair<MyVertex *, MyVertex *>(x, y));
}

void
BronKerbosch1(std::set<MyVertex *> R, std::set<MyVertex *> P,
	std::set<MyVertex *> X,
	MyGraph *g,
	std::list<std::set<MyVertex *> > &results)
{
	if (P.size() == 0 && X.size() == 0) {
		if (R.size() > 0) {
			results.push_back(R);
		}
		return;
	}
	std::set<MyVertex *>::iterator iter;
	for (	iter  = P.begin();
		iter != P.end();
	)
	{
		std::set<MyVertex *> P_intersect_Nv;
		std::set<MyVertex *> X_intersect_Nv;
		std::set<MyVertex *> R_union_v;
		std::set<MyVertex *>::iterator next = iter;
		MyVertex *v = *iter;

		next++;

		R_union_v = R;
		R_union_v.insert(v);
		std::set_intersection(P.begin(), P.end(),
			(*iter)->m_Neighbours.begin(),
			(*iter)->m_Neighbours.end(),
			std::inserter(P_intersect_Nv, P_intersect_Nv.begin()));
		std::set_intersection(X.begin(), X.end(),
			(*iter)->m_Neighbours.begin(),
			(*iter)->m_Neighbours.end(),
			std::inserter(X_intersect_Nv, X_intersect_Nv.begin()));
		BronKerbosch1(R_union_v, P_intersect_Nv, X_intersect_Nv, g, results);
		X.insert(*iter);
		P.erase(iter);
		iter = next;
	}
}

static void
Usage(const char *filename)
{
	fprintf(stderr, "Usage: %s filename\n", filename);
	exit(1);
}

void
PrintClique(std::set<MyVertex *> &x)
{
	std::set<MyVertex *>::iterator iter;

	for (	iter  = x.begin();
		iter != x.end();
		iter++)
	{
		MyStringVertex *v = static_cast<MyStringVertex *>(*iter);
		printf("%s\n", v->m_String.c_str());
		fflush(stdout);
	}
	printf("=\n");
	fflush(stdout);
}

void
PrintCliqueList(std::list<std::set<MyVertex *> > &cliqueList)
{
	std::list<std::set<MyVertex *> >::iterator iter;
	unsigned int i;

	for (	iter  = cliqueList.begin(), i = 0;
		iter != cliqueList.end();
		iter++, i++)
	{
		printf("Clique %u/%i\n", i, cliqueList.size());
		fflush(stdout);
		PrintClique(*iter);
	}
	printf("==\n");
	fflush(stdout);
}

int
main(int argc, char *argv[])
{
	std::ifstream stream;
	const char *argv0 = argv[0];
	MyGraph myGraph;

	if (argc != 2) {
		Usage(argv0);
	}
	stream.open(argv[1]);
	if (!stream) {
		fprintf(stderr, "Can't open %s\n", argv[1]);
		exit(1);
	}
	std::map<std::string, MyStringVertex *> vertexMap;
	while (!stream.eof()) {
		char s[1024];

		stream.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		std::string str(s);
		size_t n = str.find_first_of(" ");
		if (n == std::string::npos) {
			fprintf(stderr, "Corrupt\n");
			exit(1);
		}
		std::string v1 = str.substr(0, n);
		std::string v2 = str.substr(n + 1);

		if (vertexMap.find(v1) == vertexMap.end()) {
			vertexMap[v1] = new MyStringVertex(v1);
		}
		if (vertexMap.find(v2) == vertexMap.end()) {
			vertexMap[v2] = new MyStringVertex(v2);
		}
		myGraph.addEdge(vertexMap[v1], vertexMap[v2]);
	}
	printf("Original graph vertices: %u\n", myGraph.m_Verticies.size());
//	myGraph.partitionByOutdegree(5);
	std::list<MyGraph *> components;
	myGraph.getComponents(components);
	printf("Number of components is %u\n", components.size());
	fflush(stdout);
	std::list<MyGraph *>::iterator iter, iter2;
	unsigned int i;
	for (	iter  = components.begin(), i =0;
		iter != components.end();
		iter++, i++)
	{
		std::set<MyVertex *> R, P, X;
		std::list<std::set<MyVertex *> > cliqueList;

		printf("Component %u/%u\n", i, components.size());
		printf("num vertices %i\n", (*iter)->m_Verticies.size());
		fflush(stdout);

		if ((*iter)->m_Verticies.size() > PARTITION_SIZE) {
			std::list<MyGraph *> partitions;

			(*iter)->partition((*iter)->m_Verticies.size() / PARTITION_SIZE, partitions);
			printf("Partitioning..\n");
			unsigned int j;
			for (	iter2  = partitions.begin(), j =0;
				iter2 != partitions.end();
				iter2++, j++)
			{
				printf("Partition size %u\n", (*iter2)->m_Verticies.size());
				fflush(stdout);
				BronKerbosch1(R, (*iter2)->m_Verticies, X, *iter2, cliqueList);
				PrintCliqueList(cliqueList);
				delete *iter2;
			}
		} else {
			BronKerbosch1(R, (*iter)->m_Verticies, X, *iter, cliqueList);
			PrintCliqueList(cliqueList);
		}
	}
	return 0;
}
