#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void interpretArg(int argc, char* argv[],
                    FILE **input, FILE **config, int *alignment,
                    int *match, int *mismatch, int *h, int *g)
{
    char buf[16];

    if (argc < 3)
	{
		printf("ERROR: Missing arguements\n");
		return;
	}

	*input = fopen(argv[1], "r");
	if(!(*input))
	{
		printf("ERROR: Failed to open input file\n");
		return;
	}

	*alignment = atoi(argv[2]);
	if (*alignment < 0 || *alignment > 1)
	{
		printf("ERROR: Alignment out of range\n");
		return;
	}

	if (argc > 3)
		*config = fopen(argv[3], "r");
	else
		*config = fopen("parameters.config", "r");


	if(!(*config))
	{
		printf("ERROR: Failed to open config file\n");
		return;
	}

    fscanf(*config, "%s", buf);
    fscanf(*config, "%d", match);

    fscanf(*config, "%s", buf);
    fscanf(*config, "%d", mismatch);

    fscanf(*config, "%s", buf);
    fscanf(*config, "%d", h);

    fscanf(*config, "%s", buf);
    fscanf(*config, "%d", g);
}

void determineLengths(FILE *input, int *m, int *n)
{
	char* buf = NULL;
    size_t len = 0;
	int read = 0;

    getline(&buf, &len, input);
	while((read = getline(&buf, &len, input)) > 1)
	    *m += (read - 1);

	getline(&buf, &len, input);
	while((read = getline(&buf, &len, input)) > 1)
	    *n += (read - 1);
}

void determineSequences(FILE * input,
                        char **s1header, char **s1, int m, 
                        char **s2header, char **s2, int n)
{
	char* buf = NULL;
    size_t len = 0;
	int read = 0;

    read = getline(&buf, &len, input);
    *s1header = (char*)malloc(sizeof(char) * read);
    strncpy(*s1header, &buf[1], read - 2);

    *s1 = (char*)malloc(sizeof(char) * m + 1);
	while((read = getline(&buf, &len, input)) > 1)
        strncat(*s1, buf, read - 1);

    read = getline(&buf, &len, input);
    *s2header = (char*)malloc(sizeof(char) * read);
    strncpy(*s2header, &buf[1], read - 2);

    *s2 = (char*)malloc(sizeof(char) * n + 1);
	while((read = getline(&buf, &len, input)) > 1)
        strncat(*s2, buf, read - 1);
}

int createTable(Cell ***table, int m, int n)
{
    (*table) = (Cell**)malloc(sizeof(Cell*) * (m + 1));

    for(int i = 0; i < m + 1; i++)
        (*table)[i] = (Cell*)malloc(sizeof(Cell) * (n + 1));
}

int detScore(char s1, char s2, int match, int mismatch)
{
    if(s1 == s2)
        return match;
    return mismatch;
}

int maxIns(Cell c, int h, int g)
{
    int max = c.del + g + h;

    if(max < c.sub + g + h)
        max = c.sub + g + h;
    if(max < c.ins + g)
        max = c.ins + g;
    
    return max;
}

int maxSub(Cell c, char s1, char s2, int match, int mismatch)
{
    int max = c.del;

    if(max < c.sub)
        max = c.sub;
    if(max < c.ins)
        max = c.ins;
    
    return max + detScore(s1, s2, match, mismatch);
}

int maxDel(Cell c, int h, int g)
{
    int max = c.del + g;

    if(max < c.sub + g + h)
        max = c.sub + g + h;
    if(max < c.ins + g + h)
        max = c.ins + g + h;
    
    return max;
}

int newMaxIns(Cell c, int h, int g)
{
    if(c.ins + g >= c.sub + g + h && c.ins + g >= c.del + g + h )
        return c.ins;
    if(c.sub + g + h  >= c.ins + g && c.sub + g + h  >= c.del + g + h )
        return c.sub;
    else
        return c.del;
}

void printScore(int match, int mismatch, int h, int g,
                    int score, int len, int nmatches, int nmismatches, int ngaps, int nopens,
                    char *s1header, char *s1, int m, char *s1revseq, 
                    char *s2header, char *s2, int n, char *s2revseq)
{
    int i = 1;
    int j = 1;
    int k = 1;
    int igap = 0;
    int kgap = 0;
    int count = -1;
    len++;
        
    int c = 0;

    printf("Scores: matches = %d, mismatches = %d, h = %d, h = %d\n\n", match, mismatch, h, g);

    printf("Sequence 1 = \"%s\", length = %d\n", s1header, m);
    printf("Sequence 2 = \"%s\", length = %d\n\n", s2header, n);

    while(i < len)
    {
        count = i;

        printf("s1 %6d   ", i - igap);
        for(i; i - count < 60 && i < len; i++)
        {
            printf("%c", s1revseq[len - i - 1]);

            if(s1revseq[len - i - 1] == '-')
                igap++;
        }

        printf("   %d\n", i - igap - 1);

        printf("            ");
        for(j; j - count < 60 && j < len; j++)
        {
            if(s1revseq[len - j - 1] == s2revseq[len - j - 1])
                printf("|");
            else
                printf(" ");
        }
        printf("\n");

        printf("s2 %6d   ", k - kgap);
        for(k; k - count < 60 && k < len; k++)
        {
            printf("%c", s2revseq[len - k - 1]);

            if(s2revseq[len - k - 1] == '-')
                kgap++;
        }

        printf("   %d\n\n", k - kgap - 1);
    }
}