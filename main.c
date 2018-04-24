#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "help.h"
#include "global.h"
#include "local.h"

int main(int argc, char* argv[])
{
	FILE *input = NULL;
	FILE *config = NULL;
	FILE *output = NULL;

	int alignment = 0;
	int match = 0;
	int mismatch = 0;
	int h = 0;
	int g = 0;

	char *s1header = NULL;
	char *s1 = NULL;
	int m = 0;

	char *s2header = NULL;
	char *s2 = NULL;
	int n = 0;

	Cell **table;

	int len = 0;
	int nmatches = 0;
	int nmismatches = 0;
	int ngaps = 0;
	int nopens = 0;
	int pmatches = 0;
	int pgaps;


	char *s1revseq = NULL;
	char *s2revseq = NULL;

	int score = 0;
	int imax = 0;
	int jmax = 0;

	interpretArg(argc, argv,
                    &input, &config, &alignment,
                    &match, &mismatch, &h, &g);

	determineLengths(input, &m, &n);

	rewind(input);

	determineSequences(input,
                        &s1header, &s1, m, 
                        &s2header, &s2, n);

	createTable(&table, m, n);

	if(alignment == GLOBAL)
	{
		initializeGlobal(table, h, g, m, n);

		recurrenceGlobal(table, match, mismatch, h, g,
                         s1, m, 
                         s2, n);

		retraceGlobal(table, match, mismatch, h, g,
                    s1, m, &s1revseq, 
                    s2, n, &s2revseq,
                    &score, &nmatches, &nmismatches, &ngaps, &nopens);
	}

	else
	{
		initializeLocal(table, m, n);

		recurrenceLocal(table, match, mismatch, h, g,
                         s1, m, &imax,
                         s2, n, &jmax);

		retraceLocal(table, match, mismatch, h, g,
                    s1, imax, &s1revseq, 
                    s2, jmax, &s2revseq,
                    &score, &nmatches, &nmismatches, &ngaps, &nopens);

	}

	len = nmatches + nmismatches + ngaps;
	pmatches = 100 * (double)nmatches / len;
	pgaps = 100 * (double)ngaps / len;

	printScore(match, mismatch, h, g,
                    score, len, nmatches, nmismatches, ngaps,  nopens,
                    s1header, s1, m, s1revseq, 
                    s2header, s2, n, s2revseq);

	if(alignment == GLOBAL)
		printf("Global optimal alignment: %d\n", score);//, (match * nmatches) + (mismatch * nmismatches) + (g * ngaps) + (h  * nopens));
	else
		printf("Local optimal alignment: %d\n", score);//, (match * nmatches) + (mismatch * nmismatches) + (g * ngaps) + (h  * nopens));

	printf("Number of: matches = %d, mismatches = %d, gaps = %d, opening gaps = %d\n",
						nmatches, nmismatches, ngaps, nopens);

	printf("Identities = %d/%d(%%%d), Gaps = %d/%d(%%%d)\n",
						nmatches, len, pmatches, ngaps, len, pgaps);
}