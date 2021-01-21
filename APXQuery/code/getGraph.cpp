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
#include "deTruss.h"

/* double no direct */
myG g_baseG;
LIST_DECOMP_G g_lstDecompG;
long double g_ldAccu;
char *g_pcDataLabel;
map <int, bool> mpNK;

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

    g_baseG.simpleInit();
    file_fillG(szFileName, 0, g_baseG);
    g_baseG.otherInit();
    return 0;
}

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
    int iEDown = 0;
    int iTpDown = 0;
    int iEUp = 0;
    map <int, bool>::iterator itmpI;

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

    iEDown = iDownQuo * iKSpace + iRe;
    iEUp = iUpQuo * iKSpace + iRe;

    /* handle no truss */
    //printf("DEBUG up before: %d\n", iEUp);
    if (iEUp > iKMax)
    {
        iEUp = iKMax;
    }
    else
    {
        itmpI = mpNK.lower_bound(iEUp);
        iEUp = itmpI->first;
    }
    //printf("DEBUG up after: %d\n", iEUp);
    itmpI = mpNK.upper_bound(iEDown);
    itmpI--;
    iEDown = itmpI->first;
    //printf("debug down: %d\n", iEDown);
    debug_assert(2 <= iEDown);
    *piUpK = iEUp;
    *piDownK = iEDown;

    if (iEUp != iEDown)
    {
        printf("Unfinished function\n");
        debug_assert(0);
    }

    return 0;
}

/*****************
input:
        map <int, MAP_PB_G> &mpAllPbG
        int k
        long double ldR
        int iKSpace
        int iKMax
        char *pcDataLabel
        MAP_BASIC_G mpRes
description:
        get k border
******************/
int getQuery(map <int, MAP_PB_G> &mpAllPbG, int k, long double ldR, int iKSpace, long double ldMiniP, int iKMax, char *pcDataLabel, MAP_BASIC_G &mpRes)
{
    int iUpK = 0;
    int iDownK = 0;
	MAP_BASIC_G mpUpG;
	myG mpDownG;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lInitTime = 0;
	long lDeTrussTime = 0;
	long lDeleteTime = 0;

    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    getBorder(&iUpK, &iDownK, iKSpace, iKMax, k);
    // printf("kmax: %d k: %d space: %d up: %d down: %d\n",
    //        iKMax, k, iKSpace, iUpK, iDownK);

    mpDownG.simpleInit();
    mpDownG.init(g_baseG);
    delE_getGFromVal(mpUpG, mpDownG, iUpK, iDownK, mpAllPbG, ldMiniP, ldR, g_baseG);

    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lInitTime += lCurTime - lStartTime;

    lStartTime = lCurTime;

    if ((k != iDownK) || (ldR < ldMiniP))
    {
        deTruss_byK(mpDownG, k);
    }

    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lDeTrussTime += lCurTime - lStartTime;

    lStartTime = lCurTime;

    /*printf("down size: %d up size: %d\n",
           mpDownG.m_mpBasicG.size(), mpUpG.size());*/
    delE_oneGByOnce(k - 2, mpDownG, mpUpG, ldR);

    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lDeleteTime += lCurTime - lStartTime;

    mpRes.swap(mpDownG.m_mpBasicG);
    mpDownG.destroy();

    // printf("query init: %ld ms deTruss: %ld ms del: %ld ms\n",
    //        lInitTime, lDeTrussTime, lDeleteTime);

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
    int iKSpace = 0;
    int iKMax = 0;
    long double ldR = 0;
	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lQueryTime = 0;
	long lTotalTime = 0;
	long double ldMinP = 0;
	map <int, MAP_PB_G> mpAllPbG;
	char *pcResultPath = NULL;
	char *pcIndexPath = NULL;
	char *pcSrcFile = NULL;
	char *pcQueryFile = NULL;
	list<pair<int, float> > lstQuery;
	list<pair<int, float> >::iterator itlstI;
	MAP_BASIC_G mpRes;

    if (8 != argc)
    {
        printf("argc: %d\n", argc);
        debug_assert(0);
    }

    pcResultPath = argv[1];
    pcSrcFile = argv[2];
    sscanf(argv[3], "%d", &iKSpace);
    sscanf(argv[4], "%Le", &ldMinP);
    g_pcDataLabel = argv[5];
    pcQueryFile = argv[6];
    pcIndexPath = argv[7];

    init(pcSrcFile);
    iKMax = file_readDeTruss(g_lstDecompG, pcIndexPath, mpNK);
    file_getAllPbG(mpAllPbG, pcIndexPath, iKMax);
	file_readQuery(lstQuery, pcQueryFile);

	for (itlstI = lstQuery.begin(); itlstI != lstQuery.end(); itlstI++)
    {
        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

        k = itlstI->first;
        ldR = itlstI->second;
        getQuery(mpAllPbG, k, ldR, iKSpace, ldMinP, iKMax, g_pcDataLabel, mpRes);

        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lQueryTime = lCurTime - lStartTime;
        lTotalTime += lQueryTime;
        printf("query (%d, %Le) time: %ld ms\n", k, ldR, lQueryTime);

        file_saveOneRes(k, ldR, mpRes, pcResultPath, g_pcDataLabel);
    }

	printf("query APX total Time=%ld ms\n", lTotalTime);

    return 0;
}
