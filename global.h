#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initializeGlobal(Cell **table, int h, int g, int m, int n)
{
    table[0][0].ins = -999999;
    table[0][0].sub = 0;
    table[0][0].del = -999999;

    for(int i = 1; i < m + 1; i++)
    {
        table[i][0].ins = -999999;
        table[i][0].sub = -999999;
        table[i][0].del = i * g + h;
    }
    
    for(int j = 1; j < n + 1; j++)
    {
        table[0][j].ins = j * g + h;
        table[0][j].sub = -999999;
        table[0][j].del = -999999;
    }
}

void recurrenceGlobal(Cell **table, int match, int mismatch, int h, int g,
                        char *s1, int m, 
                        char *s2, int n)
{
    for(int i = 1; i < m + 1; i++)
    {
        for(int j = 1; j < n + 1; j++)
        {
            table[i][j].ins = maxIns(table[i][j - 1], h, g);

            table[i][j].sub = maxSub(table[i - 1][j - 1], s1[i - 1], s2[j - 1], match, mismatch);

            table[i][j].del = maxDel(table[i - 1][j], h, g);
        }
    }
}

void retraceGlobal(Cell **table, int match, int mismatch, int h, int g,
                    char *s1, int m, char **s1revseq, 
                    char *s2, int n, char **s2revseq,
                    int *score, int *nmatches, int *nmismatches, int *ngaps, int *nopens)
{
    int i = m;
    int j = n;
    int k = 0;

    int ins = 0;
    int sub = 0;
    int del = 0;

    int curr = 0;
    int prev = -1;

    *s1revseq = (char*)malloc(sizeof(char) * (m + n));
    *s2revseq = (char*)malloc(sizeof(char) * (m + n));

    *score = table[m][n].ins;
    if(*score < table[m][n].sub)
        *score = table[m][n].sub;
    if(*score < table[m][n].del)
        *score = table[m][n].del;

    while(i > 0 || j > 0)
    {

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