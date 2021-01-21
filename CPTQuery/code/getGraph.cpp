/***************
get graph
second step
input k and r
main cpp
****************/
#include <sys/time.h>
#include "common.h"
#include "myG.h"
#include "file.h"
#include "delE.h"
//#include "deTruss.h"

/* double no direct */
//myG g_baseG;
LIST_DECOMP_G g_lstDecompG;
char *g_pcDataLabel;

/*****************
input:
        char *szFileName source data file name
        long double ldMinP
description:
        global init
******************/
int init(char *szFileName)
{
    g_lstDecompG.setOrderOffset(getStructOffset(TPST_DECOMP_G, k), NULL);

    //g_baseG.simpleInit();
    //file_fillG(szFileName, 0, g_baseG);
    //g_baseG.otherInit();
    return 0;
}
#if 0
/*****************
input:
        int *piUpK
        int *piDownK
        int iKSpace
        int iKMax
        int k
description:
        get k border
******************/
int getBorder(int *piUpK, int *piDownK, int iKSpace, int iKMax, int k)
{
    int iRe = 0;
    int iDownQuo = 0;
    int iUpQuo = 0;
    int iPo = 0;

    iRe = iKMax % iKSpace;
    iPo = k - iRe;
    iDownQuo = iPo / iKSpace;
    if (0 == iPo % iKSpace)
    {
        iUpQuo = iDownQuo;
    }
    else
    {
        iUpQuo = iDownQuo + 1;
    }

    *piDownK = iDownQuo * iKSpace + iRe;
    *piUpK = iUpQuo * iKSpace + iRe;

    return 0;
}
#endif
/*****************
input:
        map <int, MAP_PB_G> &mpAllPbG
        int k
        long double ldR
        char *pcDataLabel
        MAP_BASIC_G mpRes
description:
        get k border
******************/
int getQuery(map <int, MAP_PB_G> &mpAllPbG, int k, long double ldR, char *pcDataLabel, MAP_BASIC_G &mpRes)
{
	MAP_BASIC_G mpUpG;

    delE_getGFromVal(mpUpG, k, mpAllPbG, ldR);

    mpRes.swap(mpUpG);
    mpUpG.clear();

    return 0;
}
/*****************
input:
        output result path name
        source database Graph file name
        int kSpace
        long double minimum possibility
        dataset label
        query file
        index path
description:
        main function
******************/
int main(int argc, char *argv[])
{
    int k = 0;
    int iKMax = 0;
    long double ldR = 0;
	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lQueryTime = 0;
	long lTotalTime = 0;
	map <int, MAP_PB_G> mpAllPbG;
	char *pcResultPath = NULL;
	char *pcIndexPath = NULL;
	char *pcSrcFile = NULL;
	char *pcQueryFile = NULL;
	list<pair<int, float> > lstQuery;
	list<pair<int, float> >::iterator itlstI;
	MAP_BASIC_G mpRes;

    if (6 != argc)
    {
        printf("argc: %d\n", argc);
        debug_assert(0);
    }

    pcResultPath = argv[1];
    pcSrcFile = argv[2];
    g_pcDataLabel = argv[3];
    pcQueryFile = argv[4];
    pcIndexPath = argv[5];

    init(pcSrcFile);
	iKMax = file_readQuery(lstQuery, pcQueryFile);
    file_getAllPbG(mpAllPbG, pcIndexPath, iKMax);

	for (itlstI = lstQuery.begin(); itlstI != lstQuery.end(); itlstI++)
    {
        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

        k = itlstI->first;
        ldR = itlstI->second;
        getQuery(mpAllPbG, k, ldR, g_pcDataLabel, mpRes);

        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lQueryTime = lCurTime - lStartTime;
        lTotalTime += lQueryTime;
        printf("query (%d, %Le) time: %ld ms\n", k, ldR, lQueryTime);

        file_saveOneRes(k, ldR, mpRes, pcResultPath, g_pcDataLabel);
    }

	printf("query %s Time=%ld ms\n", g_pcDataLabel, lTotalTime);

    return 0;
}
