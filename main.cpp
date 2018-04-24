#include <iostream>
#include <string.h>

#ifndef HELP_CPP
#define HELP_CPP

#include "help.cpp"

#endif

#ifndef ALIGNMENT_CPP
#define ALIGNMENT_CPP

#include "alignment.cpp"

#endif

#include "global.cpp"
#include "local.cpp"

using namespace std;

int main(int argc, char* argv[])
{
	Alignment *alignment;

	int match = 0;
	int mismatch = 0;
	int h = 0;
	int g = 0;

	string s1header;
	string s1;
	int m = 0;

	string s2header;
	string s2;
	int n = 0;

	determineArgs(argc, argv,
                    &s1header, &s1, &m,
                    &s2header, &s2, &n,
                    &match, &mismatch, &h, &g);

	if(!strcmp(argv[2], "0"))
		alignment = new GlobalAlignment(stoi(argv[2]), match, mismatch, h, g,
                        	&s1header, &s1, m,
                        	&s2header, &s2, n);
	else
		alignment = new LocalAlignment(stoi(argv[2]), match, mismatch, h, g,
                        	&s1header, &s1, m,
                        	&s2header, &s2, n);

	alignment->createTable();
	alignment->initialize();
	alignment->recurrence();
	alignment->retrace();

	alignment->printScore();
}