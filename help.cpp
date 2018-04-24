#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>

#ifndef ALIGNMENT_CPP
#define ALIGNMENT_CPP

enum {
	GLOBAL,
	LOCAL
};

enum {
    SUB,
    INS,
    DEL
};

typedef struct cell {
	int ins;
    int sub;
    int del;
}Cell;

#include "alignment.cpp"

#endif

using namespace std;

void determineArgs(int argc, char* argv[],
                    string *s1header, string *s1, int *m,
                    string *s2header, string *s2, int *n,
                    int *match, int *mismatch, int *h, int *g)
{
    ifstream inputStream;
	ifstream configStream;

	inputStream.open(argv[1]);

    if(argc > 3)
	    configStream.open(argv[3]);
    else
        configStream.open("parameters.config");

	string tmp;

	getline(inputStream, tmp);
	*s1header = tmp.substr(1);

	while(1)
	{
		getline(inputStream, tmp);

        if(!tmp.length())
            break;

		*s1 += tmp;
	}

    *m = s1->length();

    getline(inputStream, tmp);
	*s2header = tmp.substr(1);

	while(!inputStream.eof())
	{
		getline(inputStream, tmp);
		*s2 += tmp;
	}

    *n = s2->length();

    configStream >> tmp >> *match;
    configStream >> tmp >> *mismatch;
    configStream >> tmp >> *h;
    configStream >> tmp >> *g;
}