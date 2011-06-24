#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>
#include <map>
#include <cstdlib>

static std::string
BasePackage(std::string str)
{
        std::string name1, name2;
        size_t n;
        bool redhat = false, debian = false;

	n = str.find_last_of("/");
        if (n != std::string::npos) {
		str = str.substr(n + 1);
	}
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

static std::map<std::string, std::set<std::string> > g_Cache;

static bool
match(const std::set<std::string> &x, const std::set<std::string> &y, double threshold, double &sim)
{
	std::set<std::string>::iterator iter;
	double s, dice, overlap, jaccard;
	unsigned int match;

	match = 0;
	for (	iter  = x.begin();
		iter != x.end();
		iter++)
	{
		if (y.find(*iter) != y.end())
			match++;
	}
	dice = 2.0 * (double)match / (double)(x.size() + y.size());
	jaccard = dice / (2.0 - dice);
//	overlap = (double)match / (double)(std::min(x.size(), y.size()));
	s = jaccard;
	sim = s;
	if (s >= threshold) {
		return true;
	}
	return false;
}

static int
LoadFile(const char *filename)
{
	std::ifstream stream;

	stream.open(filename);
	if (!stream)
		exit(1);
	while (!stream.eof()) {
		char s[1024];

		stream.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		std::string name1(s);
		size_t n = name1.find_last_of("/");
		std::string name = name1.substr(n + 1);
		g_Cache[std::string(filename)].insert(name);
	}
	return 0;
}

int
LoadDistro(const char *filename, std::list<std::string> &distroList)
{
	std::ifstream stream;

	stream.open(filename);
	if (!stream)
		exit(1);
	while (!stream.eof()) {
		char s[1024];

		stream.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		LoadFile(s);
		distroList.push_back(s);
	}
	stream.close();
	return 0;
}

static void doit1(char *argv[]);
static void doit2(char *argv[]);
static void doit3(char *argv[]);

int
main(int argc, char *argv[])
{
	int ch;
	int mode;

	mode = 1;
	while ((ch = getopt(argc, argv, "123")) != EOF) {
		switch (ch) {
		case '1':
			mode = 1;
			break;

		case '2':
			mode = 2;
			break;

		case '3':
			mode = 3;
			break;

		default:
			exit(1);
		}
	}
	argc -= optind;
	argv += optind;

	switch (mode) {
	case 1:
		doit1(argv);
		break;

	case 2:
		doit2(argv);
		break;

	case 3:
		doit3(argv);
		break;
	}

	return 0;
}

static void
doit1(char *argv[])
{
	std::list<std::string> d1, d2;
	std::list<std::string>::iterator d1_iter, d2_iter;

	LoadDistro(argv[0], d1);
	LoadDistro(argv[1], d2);

	unsigned int i = 0;
	for (	d1_iter  = d1.begin();
		d1_iter != d1.end();
		d1_iter++, i++)
	{
		double bestS = 0.0;
		std::string bestN;

		printf("# %u/%u\n", i, d1.size());
		fflush(stdout);
		for (	d2_iter  = d2.begin();
			d2_iter != d2.end();
			d2_iter++)
		{
			double s;

			match(g_Cache[*d1_iter], g_Cache[*d2_iter], 0.7, s);
			if (s >= bestS) {
				bestN = *d2_iter;
				bestS = s;
			}
		}
		printf("Equivalent:%s:%s:%f\n", BasePackage(*d1_iter).c_str(), BasePackage(bestN).c_str(), bestS);
	}
}

static void
doit2(char *argv[])
{
	std::list<std::string> d1;
	std::list<std::string>::iterator d1_iter, d2_iter;

	LoadDistro(argv[0], d1);

	unsigned int i = 0;
	for (	d1_iter  = d1.begin();
		d1_iter != d1.end();
		d1_iter++, i++)
	{
		std::string res = BasePackage(*d1_iter);

		printf("# %u/%u\n", i, d1.size());
		fflush(stdout);
		for (	d2_iter  = d1.begin();
			d2_iter != d1.end();
			d2_iter++)
		{
			double s;

			if (*d1_iter != *d2_iter) {
				if (match(g_Cache[*d1_iter], g_Cache[*d2_iter], 0.8, s)) {
					char str[4096];
					snprintf(str, sizeof(str), "%f", s);
					res += std::string(" ") + BasePackage(*d2_iter) + ":" + std::string(str);
				}
			}
		}
		printf("%s\n", res.c_str());
	}
}

static void
doit3(char *argv[])
{
	std::list<std::string> d1, d2;
	std::list<std::string>::iterator d1_iter, d2_iter;
	std::map<std::string, std::set<std::string> > g_Cache2;
	std::set<std::string> ignoreList;

	LoadDistro(argv[0], d1);
	LoadDistro(argv[1], d2);

	unsigned int i = 0;
	for (	d2_iter  = d2.begin();
		d2_iter != d2.end();
		d2_iter++, i++)
	{
		std::string res = BasePackage(*d2_iter);

		if (ignoreList.find(res) == ignoreList.end()) {
			if (g_Cache2.find(res) != g_Cache2.end()) {
				ignoreList.insert(res);
				g_Cache2.erase(res);
			} else {
				g_Cache2[res] = g_Cache[*d2_iter];
			}
		}
	}
	i = 0;
	for (	d1_iter  = d1.begin();
		d1_iter != d1.end();
		d1_iter++, i++)
	{
		std::string b = BasePackage(*d1_iter);

		printf("# %u/%u\n", i, d1.size());
		fflush(stdout);
		if (g_Cache2.find(b) != g_Cache2.end()) {
			double s;
			if (match(g_Cache[*d1_iter], g_Cache2[b], 0.4, s) == false) {
				printf("Different:%s:%f\n", b.c_str(), s);
			}
		}
	}
}
