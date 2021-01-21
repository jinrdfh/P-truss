/***************
check result
main cpp
****************/
#include "common.h"
#include "file.h"
#include "delE.h"
#include "deTruss.h"
#include "calPek.h"
#include "show.h"

MAP_G g_initG;
MAP_G g_desG;
map<int, list<int> > g_maplstPNe;
pair <int, int> *ppaFindEdgeByEid;

/*****************
input:
        int k
        long double ldR
description:
        check dte
******************/
int checkDtE(int k, long double ldR)
{
    MAP_G::iterator itMap;
    long double ldDte = 0;
    TPST_MAP_BY_XY stTempMapE;
    vector <long double>::iterator itve;
    int i = 0;

    for (itMap = g_desG.begin(); itMap != g_desG.end(); itMap++)
    {
        stTempMapE = itMap->second;
        ldDte = delE_getDtE(k - 2, stTempMapE.vldPek, stTempMapE.p);
        if (ldDte < ldR)
        {
            printf("get x: %d, y: %d, sup: %d, p: %Lf, dte: %Lf\n",
                   itMap->first.first, itMap->first.second,
                   stTempMapE.iSup, stTempMapE.p, ldDte);
            for (itve = stTempMapE.vldPek.begin(); itve != stTempMapE.vldPek.end(); itve++)
            {
                printf("v[%d]: %Lf\n", i, *itve);
                i++;
            }
            show_NeibP(itMap->first.first, itMap->first.second, g_desG, g_maplstPNe);
            debug_assert(0);
        }
    }
    return 0;
}
/*****************
input:
        int k
description:
        check sup
******************/
int checkSup(int k)
{
    MAP_G::iterator itMap;

    for (itMap = g_desG.begin(); itMap != g_desG.end(); itMap++)
    {
        if (itMap->second.iSup + 2 < k)
        {
            printf("now k: %d\n", itMap->second.iSup + 2);
            debug_assert(0);
        }
    }
    return 0;
}
/*****************
input:
        int k
        char *pSName
        char *pRName
description:
        init global variables g_initG g_maplstPNe ppaFindEdgeByEid
******************/
int init(int k, char *pSName, char *pRName)
{
    FILE *fp = NULL;
    char szBuffer[ONE_LINE_BUFFER] = {0};
    int x = 0;
    int y = 0;
    int Eid = 0;
    TPST_MAP_BY_XY stTempMapE;
    int iLock = 0;
    int iCurK = 0;

    fp = fopen(pRName, "rt");
    debug_assert(NULL != fp);

    /* init g_initG */
    file_read(pSName);

    while (fgets(szBuffer, ONE_LINE_BUFFER - 1, fp) > 0)
    {
        if (sscanf(szBuffer, "%d,%d", &x, &y) != 2)
        {
            iLock = 1;
            debug_assert(sscanf(szBuffer, "k,%d", &iCurK) == 1);
            continue;
        }

        if ((iLock == 1) && (iCurK < k))
        {
            continue;
        }

        if (g_initG.find(pair<int, int>(x, y)) != g_initG.end())
        {
            Eid++;
            stTempMapE = g_initG[pair<int, int>(x, y)];
            stTempMapE.eid = Eid;
            if (g_desG.find(pair<int, int>(x, y)) == g_desG.end())
            {
                g_desG[pair<int, int>(x, y)] = stTempMapE;
                g_desG[pair<int, int>(y, x)] = stTempMapE;
            }
            else
            {
                printf("error x: %d y: %d\n", x, y);
                debug_assert(0);
            }
        }
        else
        {
            printf("error x: %d y: %d\n", x, y);
            debug_assert(0);
        }
    }
    fclose(fp);
    printf("start init\n");
    file_GlobalInit(g_desG);

    return 0;
}

/*****************
input:
        int k
        long double ldR
        source name
        result name
description:
        main function
******************/
int main(int argc, char *argv[])
{
    int k = 0;
    long double ldR = 0;
    int x = 680;
    int y = 2384;
    int eid = 0;
    list<int> cnK;

    sscanf(argv[1], "%d", &k);
    sscanf(argv[2], "%Le", &ldR);

    printf("get k: %d, ldR: %Le\n", k, ldR);

    init(k, argv[3], argv[4]);
    de_FindSup(g_maplstPNe, g_desG);
    printf("in it: %d\n",
           g_desG.find(pair<int, int>(x, y)) != g_desG.end());
    if (g_desG.find(pair<int, int>(x, y)) != g_desG.end())
    {
        printf("eid %d\n", g_desG[pair<int, int>(x, y)].eid);
    }
    cnK = deTruss_FindComNeib(x, y, g_desG, g_maplstPNe);
    printf("k neb: %d\n", cnK.size());
    checkSup(k);
    printf("sup ok\n");
    /*calPek_all(g_desG);
    checkDtE(k, ldR);
    printf("dte ok\n");*/
    return 0;
}
