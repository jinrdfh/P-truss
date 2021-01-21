/***************
delete edge in mapG
****************/
#include <sys/time.h>
#include <math.h>
#include "common.h"
#include "myG.h"
#include "deTruss.h"
#include "calPek.h"
#include "file.h"
#include "delE.h"

/*****************
input:
        long double ldDtE
        long double ldAccu
output:
        long key DtE
description:
        get key DtE, register function
******************/
long double delEFuncKeyDtE(long double ldDtE)
{
    return ldDtE;
}
/*****************
input:
        int iSup
        HLST_DT_E &hlstDtE
        MAP_BASIC_G &mapSupG
        myG &oMpG
output:
        HLST_DT_E &hlstDtE
description:
        add G to lst
******************/
bool delEAddGToDel(int iSup, HLST_DT_E &hlstDtE, MAP_BASIC_G &mapSupG, myG &oMpG)
{
    MAP_BASIC_G::iterator itG;
    int x = 0;
    int y = 0;
    ST_DT_E stTempDtE;
    TPST_MAP_BY_XY *pstMapNode;
    int ret = 0;
    bool bError = false;
    for (itG = mapSupG.begin(); itG != mapSupG.end(); itG++)
    {
        x = itG->first.first;
        y = itG->first.second;
        if (x > y)
        {
            continue;
        }

        pstMapNode = oMpG.findNode(x, y);
        stTempDtE.iKeyByEid = pstMapNode->eid;

        if (iSup > pstMapNode->vldPek.size() - 1)
        {
            vector <long double> vfDtE;
            // printf("error x: %d y: %d sup: %d size: %d\n",
            //        x, y, iSup, pstMapNode->vldPek.size());
            bError = true;
            continue;
        }
        stTempDtE.ldDtE = delE_getDtE(iSup, pstMapNode->vldPek, pstMapNode->p);
        debug_assert(0 < stTempDtE.iKeyByEid);
        hlstDtE.insetInMp(stTempDtE);
    }
    return bError;
}
/*****************
input:
        MAP_BASIC_G &mapG
        list<pair<int, int> > &lstE
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
        long double ldR   // Tight upper bound
        myG &mapG
        list<int> &lstWaitDel
        HLST_DT_E &hlstDtE
        MAP_BASIC_G &mapCalG
output:
        list<pair<int, int> > delete pointers
description:
        delete one edge
******************/
int delE_oneE(int x, int y, int iSup, long double ldR, myG &mapG, list<int> &lstWaitDel, HLST_DT_E &hlstDtE, MAP_BASIC_G &mapCalG)
{
    map<pair<int, int>, int> mpNebE;
    map<pair<int, int>, int>::iterator itNebE;

    TPST_MAP_BY_XY *pstTempE = NULL;
    long double ldTempDtE = 0;
    int iTempEid = 0;
    vector <long double> vldPek;
    ST_DT_E stTempDte;
    bool bNeedDel = true;
    vector <int> vCalP;
    vector <int>::iterator itv;
    int iDebugRes = 0;

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

            /* not be ready to be deleted and not be protected */
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
                hlstDtE.delInMp(iTempEid);
            }
            else
            {
                /* refresh Pek */
                stTempDte.iKeyByEid = iTempEid;
                stTempDte.ldDtE = ldTempDtE;
                hlstDtE.insetInMp(stTempDte);
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
        long double ldUpDt  //next batch upper bound
        myG &mapG         include UpG, will be delete
        MAP_BASIC_G &mapUpG      up bound
        MAP_PB_G_BY_KEY &mapPbG       result
        HLST_DT_E &hlstDtE
        MAP_BASIC_G &mapCalG
output:
        MAP_PB_G_BY_KEY &mapPbG
description:
        delete one G
******************/
int delE_subG(int iSup, long double ldUpDt, myG &mapG, MAP_PB_G &mapPbG, HLST_DT_E &hlstDtE, MAP_BASIC_G &mapCalG)
{
    long double ldCurBaseDtE = 0;
    int x = 0;
    int y = 0;
    int eid = 0;
    ST_DT_E stTpDtE;
    ST_DT_E stBaseDtE;
    ST_DT_E *pstTpDtE;
    pair <int, int> paXY;
    list<int> lstTopBatch;
    list<int>::iterator ltlsI;
    TPST_MAP_BY_XY *pstNode = NULL;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lDelOneETime = 0;

    while (!hlstDtE.empty())
    {
        lstTopBatch.clear();
        stBaseDtE = hlstDtE.getTop();
        ldCurBaseDtE = delEFuncKeyDtE(stBaseDtE.ldDtE);
        /* compare batch and upDtE */
        if (ldCurBaseDtE >= ldUpDt)
        {
            /* end */
            break;
        }
        hlstDtE.getTopBatch(lstTopBatch);
        for (ltlsI = lstTopBatch.begin(); ltlsI != lstTopBatch.end(); ltlsI++)
        {
            hlstDtE.delInMp(*ltlsI);
            paXY = mapG.findByEid(*ltlsI);
            pstNode = mapG.findNode(paXY.first, paXY.second);
            debug_assert(NULL != pstNode);
            pstNode->bReadyDel = true;
            deTruss_mapSub<MAP_BASIC_G>(mapCalG, paXY.first, paXY.second);
        }

        while (!lstTopBatch.empty())
        {
            eid = lstTopBatch.front();
            lstTopBatch.pop_front();

            paXY = mapG.findByEid(eid);
            pstNode = mapG.findNode(paXY.first, paXY.second);

            if (NULL == pstNode)
            {
                /* has been deleted, can not be */
                printf("error eid: %d, (%d, %d)\n",
                       eid, paXY.first, paXY.second);
                if (mapG.m_mpBasicG.find(paXY) == mapG.m_mpBasicG.end())
                {
                    printf("not in\n");
                }
                else
                {
                    printf("in it\n");
                }
                debug_assert(0);
            }

            mapPbG[ldCurBaseDtE].push_back(paXY);

            delE_oneE(paXY.first, paXY.second, iSup, ldCurBaseDtE + 1e-16, mapG, lstTopBatch, hlstDtE, mapCalG);
        }
    }

    return 0;
}

/*****************
input:
        int iSup
        myG &mapG
        MAP_PB_G &mapPbG
        MAP_PB_G *pmapUpPbG
output:
        MAP_PB_G &mapPbG
description:
        delete one G
******************/
int delE_oneG(int iSup, myG &mapG, MAP_PB_G &mapPbG)
{
    MAP_G::iterator itG;
    MAP_BASIC_G mapCalG;
    HLST_DT_E hlstDtE;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lDelSubTime = 0;

    hlstDtE.clear();
    hlstDtE.setOrderOffset(getStructOffset(ST_DT_E, ldDtE), delEFuncKeyDtE);
    hlstDtE.setFindOffset(getStructOffset(ST_DT_E, iKeyByEid));

    /* k = 2 needn't calculate */
    if (0 == iSup)
    {
        for (itG = mapG.m_mpG.begin(); itG != mapG.m_mpG.end(); itG++)
        {
            if (itG->first.first < itG->first.second)
            {
                mapPbG[delEFuncKeyDtE(itG->second.p)].push_back(itG->first);
            }
        }
        return 0;
    }

    mapCalG = mapG.m_mpBasicG;

    calPek_part<MAP_BASIC_G>(mapG.m_mpBasicG, mapG);

    /* no error */
    debug_assert(delEAddGToDel(iSup, hlstDtE, mapG.m_mpBasicG, mapG) == false );
    /* last */

    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    /* 1 maximum */
    delE_subG(iSup, 1.1, mapG, mapPbG, hlstDtE, mapCalG);
    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lDelSubTime += lCurTime - lStartTime;
    // printf("delSub Time=%ld ms\n", lDelSubTime);

    return 0;
}

/*****************
input:
        LIST_DECOMP_G &lstDeG
        char *szSaveFile
        myG &oInitG
output:
        list<pair<int, int> > delete pointers
description:
        delete part G
******************/
int delE_partG(LIST_DECOMP_G &lstDeG, char *szSaveFile, myG &oInitG)
{
    LIST_DECOMP_G::iterator itDeG;
    MAP_PB_G mapPbG;
    int iPrevK = 0;
    myG mapG;
    myG mapPrevG;
	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lTotalTime = 0;
	long lInitTime = 0;
	long lAddTime = 0;
	long lSaveTotalTime = 0;

    mapG.simpleInit();
    mapPrevG.simpleInit();
    mapPrevG.init(oInitG);

    for (itDeG = lstDeG.begin(); itDeG != lstDeG.end(); itDeG++)
    {
        if (itDeG->k < 3)
        {
            /* needn't calculate */
        }
        else
        {
            // printf("k: %d\n", itDeG->k);

            /* clear */
            gettimeofday(&tv, NULL);
            lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            mapG.copyMap(mapPrevG);
            gettimeofday(&tv, NULL);
            lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            lInitTime = lCurTime - lStartTime;
            // printf("init G(k=%d) Time=%ld ms\n", itDeG->k, lInitTime);

            lStartTime = lCurTime;
            mapG.addBatch(itDeG->mapSubG, oInitG);
            /* save */
            mapPrevG.copyMap(mapG);

            gettimeofday(&tv, NULL);
            lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            lAddTime = lCurTime - lStartTime;
            // printf("add G(k=%d) Time=%ld ms\n", itDeG->k, lAddTime);

            /* order */
            debug_assert((iPrevK > itDeG->k) || (0 == iPrevK));
            iPrevK = itDeG->k;

            mapPbG.clear();
        }

        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        delE_oneG(itDeG->k - 2, mapG, mapPbG);
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lTotalTime = lCurTime - lStartTime;
        // printf("delete G(k=%d) Time=%ld ms\n", itDeG->k, lTotalTime);

        lStartTime = lCurTime;
        file_savePbG(itDeG->k, mapPbG, szSaveFile);
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lTotalTime = lCurTime - lStartTime;
        // printf("save delete G(k=%d) Time=%ld ms\n", itDeG->k, lTotalTime);
        lSaveTotalTime += lTotalTime;
    }
    return lSaveTotalTime;
}
