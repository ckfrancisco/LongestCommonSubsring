#include <iostream>
#include <string.h>

#ifndef ALIGNMENT_CPP
#define ALIGNMENT_CPP

#include "alignment.cpp"

#endif

using namespace std;

class GlobalAlignment : public Alignment
{
	public:
        
        GlobalAlignment(int alignment, int match, int mismatch, int h, int g,
            string *s1header, string *s1, int m,
            string *s2header, string *s2, int n):
        Alignment(alignment, match, mismatch, h, g,
            s1header, s1, m,
            s2header, s2, n)
        {

        }

        ~GlobalAlignment()
        {

        }

        int initialize()
        {
            mTable[0][0].ins = -999999;
            mTable[0][0].sub = 0;
            mTable[0][0].del = -999999;

            for(int i = 1; i < mM + 1; i++)
            {
                mTable[i][0].ins = -999999;
                mTable[i][0].sub = -999999;
                mTable[i][0].del = i * mG + mH;
            }
            
            for(int j = 1; j < mN + 1; j++)
            {
                mTable[0][j].ins = j * mG + mH;
                mTable[0][j].sub = -999999;
                mTable[0][j].del = -999999;
            }
        }

        void recurrence()
        {
            for(int i = 1; i < mM + 1; i++)
            {
                for(int j = 1; j < mN + 1; j++)
                {
                    mTable[i][j].ins = maxIns(mTable[i][j - 1]);

                    mTable[i][j].sub = maxSub(mTable[i - 1][j - 1], mS1[i - 1], mS2[j - 1]);

                    mTable[i][j].del = maxDel(mTable[i - 1][j]);
                }
            }
        }

        void retrace()
        {
            int i = mM;
            int j = mN;
            int k = 0;

            int ins = 0;
            int sub = 0;
            int del = 0;

            int curr = 0;
            int prev = -1;

            mS1RevSeq.resize(mM + mN, '\0');
            mS2RevSeq.resize(mM + mN, '\0');

            mScore = mTable[mM][mN].ins;
            if(mScore < mTable[mM][mN].sub)
                mScore = mTable[mM][mN].sub;
            if(mScore < mTable[mM][mN].del)
                mScore = mTable[mM][mN].del;

            while(i > 0 || j > 0)
            {

                if(i == 0 && j > 0)
                {
                    mS1RevSeq[k] = '-'; mS2RevSeq[k] = mS2[j - 1];

                    if(prev != INS)
                        mNOpens++;
                    mNGaps++;

                    j--; k++;
                    prev = INS;
                }

                else if(i > 0 && j == 0)
                {
                    mS1RevSeq[k] = mS1[i - 1]; mS2RevSeq[k] = '-';

                    if(prev != DEL)
                        mNOpens++;
                    mNGaps++;

                    i--; k++;
                    prev = DEL;
                }

                else
                {
                    ins = mTable[i][j].ins;
                    sub = mTable[i][j].sub;
                    del = mTable[i][j].del;

                    if(prev == INS)
                    {
                        sub += mH;
                        del += mH;
                    }

                    if(prev == DEL)
                    {
                        ins += mH;
                        sub += mH;
                    }

                    if(ins >= sub && ins >= del)
                    {
                        mS1RevSeq[k] = '-'; mS2RevSeq[k] = mS2[j - 1];

                        if(prev != INS)
                            mNOpens++;
                        mNGaps++;

                        j--; k++;
                        prev = INS;
                    }
                    else if(sub >= ins && sub >= del)
                    {
                        mS1RevSeq[k] = mS1[i - 1]; mS2RevSeq[k] = mS2[j - 1];

                        if(mS1RevSeq[k] == mS2RevSeq[k])
                            mNMatches++;
                        else
                            mNMismatches++;

                        i--; j--; k++;
                        prev = SUB;
                    }
                    else
                    {
                        mS1RevSeq[k] = mS1[i - 1]; mS2RevSeq[k] = '-';

                        if(prev != DEL)
                            mNOpens++;
                        mNGaps++;

                        i--; k++;
                        prev = DEL;
                    }
                }
            }
            
            mS1RevSeq[k] = '\0'; mS2RevSeq[k] = '\0';

            mLen = mNMatches + mNMismatches + mNGaps;
		    mPMatches = 100 * (double)mNMatches / mLen;
		    mPGaps = 100 * (double)mNGaps / mLen;
            
            return;
        }
};