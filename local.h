#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initializeLocal(Cell **table, int m, int n)
{
	table[0][0].ins = 0;
    table[0][0].sub = 0;
    table[0][0].del = 0;

    for(int i = 1; i < m + 1; i++)
    {
        table[i][0].ins = 0;
        table[i][0].sub = 0;
        table[i][0].del = 0;
    }
    
    for(int j = 1; j < n + 1; j++)
    {
        table[0][j].ins = 0;
        table[0][j].sub = 0;
        table[0][j].del = 0;
    }
}

void recurrenceLocal(Cell **table, int match, int mismatch, int h, int g,
                        char *s1, int m, int *imax, 
                        char *s2, int n, int *jmax)
{
    int scoremax = 0;

    for(int i = 1; i < m + 1; i++)
    {
        for(int j = 1; j < n + 1; j++)
        {
            table[i][j].ins = maxIns(table[i][j - 1], h, g);
            if(table[i][j].ins < 0)
                table[i][j].ins = 0;

            if(scoremax < table[i][j].ins)
            {
                scoremax = table[i][j].ins;
                *imax = i;
                *jmax = j;
            }

            table[i][j].sub = maxSub(table[i - 1][j - 1], s1[i - 1], s2[j - 1], match, mismatch);
            if(table[i][j].sub < 0)
                table[i][j].sub = 0;

            if(scoremax < table[i][j].sub)
            {
                scoremax = table[i][j].sub;
                *imax = i;
                *jmax = j;
            }

            table[i][j].del = maxDel(table[i - 1][j], h, g);
            if(table[i][j].del < 0)
                table[i][j].del = 0;

            if(scoremax < table[i][j].del)
            {
                scoremax = table[i][j].del;
                *imax = i;
                *jmax = j;
            }
        }
    }
}

void retraceLocal(Cell **table, int match, int mismatch, int h, int g,
                    char *s1, int imax, char **s1revseq, 
                    char *s2, int jmax, char **s2revseq,
                    int *score, int *nmatches, int *nmismatches, int *ngaps, int *nopens)
{
    int i = imax;
    int j = jmax;
    int k = 0;

    int ins = 0;
    int sub = 0;
    int del = 0;

    int curr = 0;
    int prev = -1;

    *s1revseq = (char*)malloc(sizeof(char) * (imax + jmax));
    *s2revseq = (char*)malloc(sizeof(char) * (imax + jmax));

    *score = table[imax][jmax].ins;
    if(*score < table[imax][jmax].sub)
        *score = table[imax][jmax].sub;
    if(*score < table[imax][jmax].del)
        *score = table[imax][jmax].del;

    while(i > 0 || j > 0)
    {

        if(table[i][j].ins == 0 && table[i][j].sub == 0 && table[i][j].del == 0)
            break;

        if(i == 0 && j > 0)
        {
            (*s1revseq)[k] = '-'; (*s2revseq)[k] = s2[j - 1];

            if(prev != INS)
                (*nopens)++;
            (*ngaps)++;

            j--; k++;
            prev = INS;
        }

        else if(i > 0 && j == 0)
        {
            (*s1revseq)[k] = s1[i - 1]; (*s2revseq)[k] = '-';

            if(prev != DEL)
                (*nopens)++;
            (*ngaps)++;

            i--; k++;
            prev = DEL;
        }

        else
        {
            ins = table[i][j].ins;
            sub = table[i][j].sub;
            del = table[i][j].del;

            if(prev == INS)
            {
                sub += h;
                del += h;
            }

            if(prev == DEL)
            {
                ins += h;
                sub += h;
            }

            if(ins >= sub && ins >= del)
            {
                (*s1revseq)[k] = '-'; (*s2revseq)[k] = s2[j - 1];

                if(prev != INS)
                    (*nopens)++;
                (*ngaps)++;

                j--; k++;
                prev = INS;
            }
            else if(sub >= ins && sub >= del)
            {
                (*s1revseq)[k] = s1[i - 1]; (*s2revseq)[k] = s2[j - 1];

                if((*s1revseq)[k] == (*s2revseq)[k])
                    (*nmatches)++;
                else
                    (*nmismatches)++;

                i--; j--; k++;
                prev = SUB;
            }
            else
            {
                (*s1revseq)[k] = s1[i - 1]; (*s2revseq)[k] = '-';

                if(prev != DEL)
                    (*nopens)++;
                (*ngaps)++;

                i--; k++;
                prev = DEL;
            }
        }
    }
    
    (*s1revseq)[k] = 0; (*s2revseq)[k] = 0;
    
    return;
}