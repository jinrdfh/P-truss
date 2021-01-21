/***************
delete edge in mapG
****************/
#include <sys/time.h>
#include <math.h>
#include "common.h"
#include "myG.h"
//#include "deTruss.h"
//#include "calPek.h"
#include "file.h"
#include "delE.h"

extern long double g_ldAccu;

/*****************
input:
        int iSup
        vector <long double> &vfPek
        float p
output:
        DtE(e, t)
description:
        get DtE(e, t)
******************/
bool delEGetGFromList(MAP_BASIC_G &mapG, list<pair<int, int> > &lstE)
{
    list< pair<int, int> >::iterator itI;
    for (itI = lstE.begin(); itI != lstE.end(); itI++)
    {
        debug_assert(mapG.find(*itI) == mapG.end());
        mapG[*itI];
        mapG[pair<int, int>(itI->second, itI->first)];
    }
    return 0;
}
#if 0
/*****************
input:
        int iSup
        vector <long double> &vfPek
        float p
output:
        DtE(e, t)
description:
        get DtE(e, t)
******************/
long double delE_getDtE(int iSup, vector <long double> &vfPek, long double p)
{
    int i = 0;
    long double ldRes = 0;
    int iMax = 0;

    iMax = vfPek.size();
    if (iSup > iMax - 1)
    {
        printf("sup: %d, size: %d\n", iSup, iMax);
        debug_assert(0);
    }

    if (iSup < iMax / 2)
    {
        for (i = 0; i < iSup; i++)
        {
            ldRes += vfPek[i];
        }
        ldRes = 1 - ldRes;
    }
    else
    {
        for (i = iSup; i < iMax; i++)
        {
            ldRes += vfPek[i];
        }
    }
    if (1e-6 < (ldRes - 1))
    {
        return -1;
    }
    return (ldRes) * p;
}

/*****************
input:
        int x
        int y
        int iSup
        long double ldR
        MAP_G &mapG
        MAP_G &mapUpG
        list<int> &lstWaitDel
        MAP_BASIC_G &mapCalG
output:
        MAP_G &mapG
description:
        delete one G
******************/
int delE_oneEByOnce(int x, int y, int iSup, long double ldR, myG &mapG, MAP_BASIC_G &mapUpG, list<int> &lstWaitDel, MAP_BASIC_G &mapCalG)
{
    map<pair<int, int>, int> mpNebE;
    map<pair<int, int>, int>::iterator itNebE;
    /*list<int> lstNeib;
    list<int>::iterator itNeib;*/
    TPST_MAP_BY_XY *pstTempE = NULL;
    long double ldTempDtE = 0;
    int iTempEid = 0;
    bool bNeedDel = true;
    vector <int> vCalP;
    vector <int>::iterator itv;

    vCalP.push_back(x);
    vCalP.push_back(y);

    if (0 < iSup)
    {
        mapG.findNebE(x, y, mpNebE, mapCalG);

        for (itNebE = mpNebE.begin(); itNebE != mpNebE.end(); itNebE++)
        {
            int u = 0;
            int v = 0;
            int w = 0;
            u = itNebE->first.first;
            w = itNebE->first.second;
            v =  itNebE->second;

            pstTempE = mapG.findNode(u, w);

            iTempEid = pstTempE->eid;
            pstTempE->iSup--;
            if (pstTempE->iSup < iSup)
            {
                /* need to be delete */;
                bNeedDel = true;
            }
            else
            {
                /* refresh ldDtE */
                bNeedDel = false;

                calPek_update(u, w, v, pstTempE->iSup, pstTempE->vldPek, mapG);
                ldTempDtE = delE_getDtE(iSup, pstTempE->vldPek, pstTempE->p);
                /* total are right, ignore */
                if (ldTempDtE < ldR)
                {
                    bNeedDel = true;
                }
            }

            if (bNeedDel)
            {
                /* need to delete */
                debug_assert(0 < iTempEid);
                lstWaitDel.push_back(iTempEid);

                pstTempE->bReadyDel = true;
                deTruss_mapSub<MAP_BASIC_G>(mapCalG, u, w);
            }
        }
    }

    /* delete x y */
    mapG.del(x, y);

    return 0;
}

/*****************
input:
        int iSup
        MAP_G &mapG
        MAP_G &mapUpG
        long double ldUpDtE
output:
        MAP_G &mapG
description:
        delete one G
******************/
int delE_oneGByOnce(int iSup, myG &mapG, MAP_BASIC_G &mapUpG, long double ldUpDtE)
{
    long double ldTempDtE = 0;
    int eid = 0;
    MAP_BASIC_G mapCalG;
    list <int> lstWaitDel;
    list <int>::iterator itLst;
    MAP_BASIC_G::iterator itG;
    int x = 0;
    int y = 0;
    TPST_MAP_BY_XY *pstMapNode;
    int ret = 0;
    pair <int, int> paXY;

    /* k = 2 needn't calculate */
    if (0 == iSup)
    {
        return 0;
    }

    deTruss_mapSub<MAP_BASIC_G>(mapCalG, mapG.m_mpBasicG, mapUpG);

    calPek_part<MAP_BASIC_G>(mapCalG, mapG);

    for (itG = mapCalG.begin(); itG != mapCalG.end(); itG++)
    {
        x = itG->first.first;
        y = itG->first.second;
        if (x > y)
        {
            continue;
        }
        pstMapNode = mapG.findNode(x, y);
        eid = pstMapNode->eid;

        /*if (iSup > pstMapNode->vldPek.size() - 1)
        {
            continue;
        }*/

        ldTempDtE = delE_getDtE(iSup, pstMapNode->vldPek, pstMapNode->p);
        if (ldTempDtE < ldUpDtE)
        {
            lstWaitDel.push_back(eid);
            pstMapNode->bReadyDel = true;
        }
    }

    for (itLst = lstWaitDel.begin(); itLst != lstWaitDel.end(); itLst++)
    {
        paXY = mapG.findByEid(*itLst);
        deTruss_mapSub<MAP_BASIC_G>(mapCalG, paXY.first, paXY.second);
    }

    while (!lstWaitDel.empty())
    {
        eid = lstWaitDel.front();
        lstWaitDel.pop_front();

        paXY = mapG.findByEid(eid);

        if (mapG.m_mpG.find(paXY) == mapG.m_mpG.end())
        {
            /* has been deleted, can not be */
            printf("error eid: %d, (%d, %d)\n",
                   eid, paXY.first, paXY.second);
            debug_assert(0);

        }

        delE_oneEByOnce(paXY.first, paXY.second, iSup, ldUpDtE, mapG, mapUpG, lstWaitDel, mapCalG);
    }

    return 0;
}
#endif
/*****************
input:
        MAP_BASIC_G &mpUpG
        int iUpK
        map <int, MAP_PB_G> &mpAllPbG
        long double ldR
        MAP_G &mpBaseG
description:
        get k border G
******************/
int delE_getGFromVal(MAP_BASIC_G &mpUpG, int iUpK, map <int, MAP_PB_G> &mpAllPbG, long double ldR)
{
    MAP_PB_G::iterator itPbG;
    long double ldUpR = 0;

    debug_assert(mpAllPbG.find(iUpK) != mpAllPbG.end());

    itPbG = mpAllPbG[iUpK].lower_bound(ldR);

    if (itPbG == mpAllPbG[iUpK].end())
    {
        /* no upper bound */
        ldUpR = 1;
    }
    else
    {
        ldUpR = itPbG->first;
        /* fill up G */
        for (; itPbG != mpAllPbG[iUpK].end(); itPbG++)
        {
            delEGetGFromList(mpUpG, itPbG->second);
        }
    }

    // printf("ldR: %Le upper: %Le\n",
    //        ldR, ldUpR);

    return 0;
}


