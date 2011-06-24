#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include <set>
#include <cstdlib>

static std::string
getStem(const char *name)
{
	unsigned int i;
	std::string str;

	i = 0;
	if (!strncmp(name, "lib", 3)) {
		i += 3;
	}
	for (; name[i];) {
		if (!strncmp(&name[i], "common", 6)) {
			i += 6;
		} else if (!strncmp(&name[i], "compat", 6)) {
			i += 6;
		} else if (name[i] == '-') {
			i++;
		} else if (name[i] == '.') {
			i++;
		} else if (isdigit(name[i])) {
			i++;
		} else {
			str += name[i];
			i++;
		}
	}
	return str;
}

static void
getList(const char *file, std::set<std::string> &dirNames)
{
	std::ifstream in;

	in.open(file);
	if (!in)
		exit(1);
	while (!in.eof()) {
		char s[1024];

		in.getline(s, sizeof(s));
		if (s[0] == 0)
			break;
		std::string str1(s);
		std::string str2 = str1.substr(0, str1.find_last_of("/"));
		std::string str3 = str2.substr(str2.find_last_of("/") + 1);
		if (str3.size() > 0) {
			std::string str4 = getStem(str3.c_str());
			if (str4.size() > 0) {
				dirNames.insert(str4);
			}
		}
	}
	in.close();
}

int
main(int argc, char *argv[])
{
	std::set<std::string> dirNames;
	std::set<std::string>::iterator dirNamesIter;
	std::string argStems[argc - 1];

	if (argc == 2) {
		printf("%s\n", getStem(argv[1]).c_str());
		exit(0);
	}
	for (int i = 2; i < argc; i++) {
		argStems[i - 2] = getStem(argv[i]);
	}
	getList(argv[1], dirNames);
	for (	dirNamesIter  = dirNames.begin();
		dirNamesIter != dirNames.end();
		dirNamesIter++)
	{
		for (int i = 0; i < argc - 2; i++) {
			if (argStems[i] == *dirNamesIter) {
				printf("%s match %s\n", argv[i + 2], dirNamesIter->c_str());
				exit(1);
			}
		}
	}
	return 0;
}
