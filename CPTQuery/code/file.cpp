/***************
input file output file function
****************/

#include "common.h"
#include "myG.h"
#include "file.h"
#include "delE.h"

extern long double g_ldAccu;
extern char *g_pcDataLabel;

int l_iMaxPid;
#if 0
/*****************
input:
        char *szFileName
        long double ldMinP
        myG &mpG
description:
        read file and fill the list g_initG
******************/
int file_fillG(char *szFileName, long double ldMinP, myG &mpG)
{
    FILE *fp = NULL;
    int iTempX = 0;
    int iTempY = 0;
    char szBuffer[ONE_LINE_BUFFER] = {0};
    long double fP = 0;

    /* replace 0 */
    int iSubPid = 4036538;

    if (NULL == szFileName)
    {
        fp = fopen(DATA_PATH, "rt");
    }
    else
    {
        fp = fopen(szFileName, "rt");
    }
    if (NULL == fp)
    {
        printf("error no file: %s or %s\n", DATA_PATH, szFileName);
        debug_assert(0);
    }
    while (fgets(szBuffer, ONE_LINE_BUFFER - 1, fp) > 0)
    {
        sscanf(szBuffer, "%d,%d,%Lf", &iTempX, &iTempY, &(fP));
        if ((fP < 0) || (fP > 1))
        {
            printf("error %d,%d,%Lf\n", iTempX, iTempY, fP);
            continue;
        }
        else if (fP < ldMinP)
        {
            continue;
        }

        if (iTempX == iTempY)
        {
            continue;
        }

        if (0 == iTempX)
        {
            iTempX = iSubPid;
        }
        if (0 == iTempY)
        {
            iTempY = iSubPid;
        }

        mpG.add(iTempX, iTempY, fP);
    }
    fclose(fp);
    return 0;
}
#endif
/*****************
input:
        MAP_PB_G &mapPbG
        char *szDesFile
description:
        get pbG
******************/
int file_getPbG(MAP_PB_G &mapPbG, char *szDesFile)
{
    FILE *fp = NULL;
    char szBuffer[ONE_LINE_BUFFER] = {0};
    list<pair<int, int> > lstCurL;
    long double ldPrevR = 0;
    long double ldR = 0;
    int x = 0;
    int y = 0;

    fp = fopen(szDesFile, "rt");
    if (NULL == fp)
    {
        /* no such file */
        return -1;
    }

    while (fgets(szBuffer, ONE_LINE_BUFFER - 1, fp) > 0)
    {
        if (sscanf(szBuffer, "r,%Lf", &ldR) == 1)
        {
            /* new r */
            if ((0 < ldPrevR) && (!lstCurL.empty()))
            {
                /* have value, save */
                mapPbG[ldPrevR] = lstCurL;
                lstCurL.clear();
            }
            ldPrevR = ldR;
        }
        else
        {
            debug_assert(sscanf(szBuffer, "%d,%d", &x, &y) == 2);
            lstCurL.push_front(pair <int, int>(x, y));
        }

    }
    /* last r */
    if ((0 < ldPrevR) && (!lstCurL.empty()))
    {
        /* have value, save */
        mapPbG[ldPrevR] = lstCurL;
        lstCurL.clear();
    }
    fclose(fp);
    return 0;
}

/*****************
input:
        map <int, MAP_PB_G> &mpAllPbG
        char *szDesPath
        int iKMax
return:
        k cnt
description:
        get all pbG
******************/
int file_getAllPbG(map <int, MAP_PB_G> &mpAllPbG, char *szDesPath, int iKMax)
{
    char szFileName[FILE_NAME_BUFFER] = {0};
    int k = 0;
    int ret = 0;
    int iKCnt = 0;

    for (k = 2; k <= iKMax; k++)
    {
        sprintf(szFileName, "%s/%s-%d-truss.index",
                szDesPath, g_pcDataLabel, k);
        ret = file_getPbG(mpAllPbG[k], szFileName);
        if (ret < 0)
        {
            /* no such file, ignore */
            mpAllPbG.erase(k);
        }
        else
        {
            iKCnt++;
        }
    }
    return iKCnt;
}
#if 0
/*****************
input:
        LIST_DECOMP_G &lstDeG
        char *szTrussPath
output:
        LIST_DECOMP_G &lstDeG
return:
        kmax
description:
        save truss decomposition
******************/
int file_readDeTruss(LIST_DECOMP_G &lstDeG, char *szTrussPath)
{
    FILE *fp = NULL;
    int x = 0;
    int y = 0;
    int k = 0;
    int kMax = 0;
    char szFileName[FILE_NAME_BUFFER] = {0};
    char szBuffer[ONE_LINE_BUFFER] = {0};
    TPST_DECOMP_G stDeG;
    TPST_MAP_BY_XY stMpNode;

    stDeG.k = 0;

    sprintf(szFileName, "%s/%s-%s", szTrussPath, g_pcDataLabel, TRUSS_FILE);
    fp = fopen(szFileName, "rt");

    if (NULL == fp)
    {
        printf("error read file: %s\n", szFileName);
        debug_assert(0);
    }

    while (fgets(szBuffer, ONE_LINE_BUFFER - 1, fp) > 0)
    {
        if (sscanf(szBuffer, "k,%d", &k) == 1)
        {
            if (0 != stDeG.k)
            {
                lstDeG.insetByOrder(stDeG);
            }
            stDeG.k = k;
            stDeG.mapSubG.clear();
            if (kMax < k)
            {
                kMax = k;
            }
        }
        else if (sscanf(szBuffer, "%d,%d", &x, &y) == 2)
        {
            stDeG.mapSubG[pair<int, int>(x, y)];
        }
        else
        {
            debug_assert(0);
        }
    }
    fclose(fp);
    fp = NULL;
    return kMax;
}
#endif
/*****************
input:
        LIST_DECOMP_G &lstDeG
        char *szSaveFile
output:
        none
description:
        save truss decomposition
******************/
int file_saveG(MAP_BASIC_G &mpG, char *szSaveFile)
{
    FILE *fp = NULL;
    MAP_BASIC_G::iterator i;
    int x = 0;
    int y = 0;

    fp = fopen(szSaveFile, "wt");

    if (NULL == fp)
    {
        printf("error read file: %s\n", szSaveFile);
        debug_assert(0);
    }

    for (i = mpG.begin(); i != mpG.end(); i++)
    {
        x = i->first.first;
        y = i->first.second;
        if (x < y)
        {
            fprintf(fp, "%d,%d\n", x, y);
        }
    }
    fclose(fp);
    fp = NULL;
}
/*****************
input:
        list<pair<int, float> > &lstQuery
        char *szSaveFile
output:
        none
description:
        read query data
******************/
int file_readQuery(list<pair<int, float> > &lstQuery, char *szSaveFile)
{
    FILE *fp = NULL;
    list<pair<int, float> >::iterator i;
    char szBuffer[ONE_LINE_BUFFER] = {0};
    int k = 0;
    int iKMax = 0;
    float fDt = 0;

    fp = fopen(szSaveFile, "rt");

    if (NULL == fp)
    {
        printf("error read file: %s\n", szSaveFile);
        debug_assert(0);
    }

    while (fgets(szBuffer, ONE_LINE_BUFFER - 1, fp) > 0)
    {
        if (sscanf(szBuffer, "%d,%f", &k, &fDt) == 2)
        {
            lstQuery.push_back(pair<int, float>(k, fDt));
            iKMax = (iKMax > k) ? iKMax : k;
        }
        else
        {
            debug_assert(0);
        }
    }
    fclose(fp);
    fp = NULL;

    return iKMax;
}
/*****************
input:
        map <pair<int, float>, MAP_G> &mpRes
        char *szResPath
        char *szLabel
output:
        none
description:
        save truss decomposition
******************/
int file_saveOneRes(int iDesK, float fDesR, MAP_BASIC_G &mpRes, char *szResPath, char *szLabel)
{
    char szFileName[FILE_NAME_BUFFER] = {0};

    sprintf(szFileName, "%s/%s-(%d,%f)-truss.txt",
            szResPath, szLabel, iDesK, fDesR);
    file_saveG(mpRes, szFileName);

    return 0;
}
