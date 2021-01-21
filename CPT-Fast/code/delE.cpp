/***************
delete edge in mapG
****************/
#include <sys/time.h>
#include <math.h>
#include "common.h"
#include "myG.h"
#include "deTruss.h"
#include "delE.h"
#include "calPek.h"
#include "file.h"

#define g_ldAccu (1e-7)
long g_lBuildVTime;
long g_lBuildSortTime;
long g_lEReSortTime;
long g_lEUpdateTime;
/*****************
input:
        long double ldDtE
        long double ldAccu
output:
        long key DtE
description:
        get key DtE, register function
******************/
MAP_E_KEY delEFuncKeyDtE(long double ldDtE)
{
    /*long double ldRes = 0;
    if (ldDtE < 0)
    {
        return 0;
    }
    ldRes = (long(ldDtE / g_ldAccu)) * g_ldAccu;
    return ldRes;*/
    MAP_E_KEY tyTep = 1 + ldDtE;
    return tyTep - 1;
}
/*****************
input:
        int iSup
        HLST_DT_E &hlstDtE
        list<int> &liSupG
        myG &oMpG
output:
        HLST_DT_E &hlstDtE
description:
        add G to lst
******************/
bool delEAddGToDel(int iSup, HLST_DT_E &hlstDtE, list<int> &liSupG, myG &oMpG)
{
    list<int>::iterator itG;
    int iDebugI = 0;

    ST_DT_E stTempDtE;
    TPST_MAP_BY_XY *pstMapNode;
    int ret = 0;
    bool bError = false;
    for (itG = liSupG.begin(); itG != liSupG.end(); itG++)
    {
        pstMapNode = oMpG.findNode(*itG);
        stTempDtE.iKeyByEid = pstMapNode->eid;

        if (iSup > pstMapNode->iVPLen - 1)
        {
            // printf("error eid: %d pEid: %d, (%d, %d) sup: %d recordSize: %d size: %d\n",
            //        *itG, pstMapNode->eid, pstMapNode->paXY.first, pstMapNode->paXY.second,
            //        iSup, pstMapNode->iVPLen, pstMapNode->vldPek.size());
            bError = true;
            continue;
        }
        stTempDtE.ldDtE = delE_getDtE(iSup, pstMapNode->vldPek, pstMapNode->p, pstMapNode->iVPLen);
        //printf("DEBUG DtE: %Le len: %d cLen: %d\n", stTempDtE.ldDtE, pstMapNode->vldPek.size(), pstMapNode->iVPLen);
        debug_assert(0 < stTempDtE.iKeyByEid);
        /*for (iDebugI = 0; iDebugI < pstMapNode->iVPLen; ++iDebugI)
        {
            printf("%Le ", pstMapNode->vldPek[iDebugI]);
        }
        printf("\n");*/
        hlstDtE.insetInMp(stTempDtE);
    }
    return bError;
}
/*****************
input:
        MAP_BASIC_G &mapG
        list<pair<int, int> > &lstE
        myG &oInitG
output:
        DtE(e, t)
description:
        get DtE(e, t)
******************/
bool delEGetGFromList(MAP_BASIC_G &mapG, list<int> &lstE, myG &oInitG)
{
    list<int>::iterator itI;
    pair<int, int> paXY;
    for (itI = lstE.begin(); itI != lstE.end(); itI++)
    {
        paXY = oInitG.findByEid(*itI);
        mapG[paXY];
        mapG[pair<int, int>(paXY.second, paXY.first)];
    }
    return 0;
}
/*****************
input:
        int iSup
        vector <long double> &vfPek
        float p
        int iVLen
output:
        DtE(e, t)
description:
        get DtE(e, t)
******************/
long double delE_getDtE(int iSup, vector <long double> &vfPek, long double p, int iVLen)
{
    int i = 0;
    long double ldRes = 0;
    //int iMax = 0;

    /*iMax = vfPek.size();
    if (iSup > iMax - 1)
    {
        printf("sup: %d, size: %d\n", iSup, iMax);
        debug_assert(0);
    }*/

    if (iSup < iVLen / 2)
    {
        for (i = 0; i < iSup; i++)
        {
            ldRes += vfPek[i];
        }
        ldRes = 1 - ldRes;
    }
    else
    {
        for (i = iSup; i < iVLen; i++)
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
int delE_oneE(int eid, int iSup, MAP_E_KEY tyR, myG &mapG, list<int> &lstWaitDel, HLST_DT_E &hlstDtE, MAP_BASIC_G &mapCalG, int iTriggerEid)
{
    LI_COM_E liNebE;
    LI_COM_E::iterator itNebE;

    TPST_MAP_BY_XY *pstTempE = NULL;
    long double ldTempDtE = 0;
    int iTempEid = 0;

    ST_DT_E stTempDte;
    bool bNeedDel = true;
    vector <int>::iterator itv;
    int iDebugRes = 0;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;

    if (0 < iSup)
    {
        mapG.findNebE(eid, liNebE, mapCalG);

        for (itNebE = liNebE.begin(); itNebE != liNebE.end(); itNebE++)
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
            debug_assert(0 != iTempEid);
            pstTempE->iSup--;

            /* save v */
            if (iTriggerEid != pstTempE->iTriggerEid)
            {
                pstTempE->iTriggerEid = iTriggerEid;
                pstTempE->iVTgLen = pstTempE->iVPLen;
                pstTempE->vldTriggerPek = pstTempE->vldPek;
            }
            if (pstTempE->iSup < iSup)
            {
                /* need to be delete */;
                bNeedDel = true;
            }
            else
            {
                /* refresh ldDtE */
                bNeedDel = false;
                //printf("DEBUG update\n");
                gettimeofday(&tv, NULL);
                lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                calPek_update(u, w, v, pstTempE->iSup, pstTempE->vldPek, mapG, &(pstTempE->iVPLen));
                gettimeofday(&tv, NULL);
                lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                g_lEUpdateTime += lCurTime - lStartTime;

                ldTempDtE = delE_getDtE(iSup, pstTempE->vldPek, pstTempE->p, pstTempE->iVPLen);
                /* total are right, ignore */
                if (ldTempDtE < tyR)
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

                gettimeofday(&tv, NULL);
                lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                hlstDtE.insetInMp(stTempDte);
                gettimeofday(&tv, NULL);
                lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                g_lEReSortTime += lCurTime - lStartTime;
            }
        }

    }
    /* delete x y */
    mapG.del(eid);
    return 0;
}

/*****************
input:
        int iSup
        long double ldUpDt  //next batch upper bound
        myG &mapG         include UpG, will be delete
        MAP_PB_G_BY_KEY &mapPbG       result
        HLST_DT_E &hlstDtE
        MAP_BASIC_G &mapCalG
output:
        MAP_PB_G_BY_KEY &mapPbG
description:
        delete one G
******************/
int delE_subG(int iSup, MAP_E_KEY tpUpDt, myG &mapG, LT_PB_G &ltPbG, HLST_DT_E &hlstDtE, MAP_BASIC_G &mapCalG)
{
    MAP_E_KEY tpCurBaseDtE = 0;
    int x = 0;
    int y = 0;
    int eid = 0;
    int iTriggerEid = 0;
    ST_DT_E stTpDtE;
    ST_DT_E stBaseDtE;
    ST_DT_E *pstTpDtE;
    pair <int, int> paXY;
    list<int> lstTopBatch;
    list<int>::iterator ltlsI;
    TPST_MAP_BY_XY *pstNode = NULL;
    TPST_PB_NODE stCurPbNode;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lDelOneETime = 0;

    while (!hlstDtE.empty())
    {
        lstTopBatch.clear();
        stBaseDtE = hlstDtE.getTop();
        tpCurBaseDtE = delEFuncKeyDtE(stBaseDtE.ldDtE);
        iTriggerEid = stBaseDtE.iKeyByEid;
        /* compare batch and upDtE */
        if (tpCurBaseDtE >= tpUpDt)
        {
            /* end */
            break;
        }
        stCurPbNode.fKey = tpCurBaseDtE;
        stCurPbNode.ltESet.clear();

        hlstDtE.getTopBatch(lstTopBatch);
        for (ltlsI = lstTopBatch.begin(); ltlsI != lstTopBatch.end(); ltlsI++)
        {
            hlstDtE.delInMp(*ltlsI);
            pstNode = mapG.findNode(*ltlsI);
            paXY = pstNode->paXY;
            debug_assert(NULL != pstNode);
            pstNode->bReadyDel = true;

            /* save v */
            pstNode->iTriggerEid = iTriggerEid;
            pstNode->iVTgLen = pstNode->iVPLen;
            pstNode->vldTriggerPek = pstNode->vldPek;

            deTruss_mapSub<MAP_BASIC_G>(mapCalG, paXY.first, paXY.second);
        }

        while (!lstTopBatch.empty())
        {
            eid = lstTopBatch.front();
            lstTopBatch.pop_front();

            pstNode = mapG.findNode(eid);
            paXY = pstNode->paXY;

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

            // mapPbG[tpCurBaseDtE].push_back(paXY);
            stCurPbNode.ltESet.push_back(eid);

            delE_oneE(eid, iSup, tpCurBaseDtE + g_ldAccu, mapG, lstTopBatch, hlstDtE, mapCalG, iTriggerEid);
        }
        ltPbG.push_back(stCurPbNode);
    }

    return 0;
}

/*****************
input:
        int iSup
        myG &mapG
        LT_PB_G &ltPbG
        LT_PB_G *pltUpPbG
        LI_XY_E &mapFreeKPart
        myG &mapUpG
output:
        LT_PB_G &ltPbG
description:
        delete one G
******************/
int delE_oneG(int iSup, myG &mapG, LT_PB_G &ltPbG, LT_PB_G *pltUpPbG, list<int> &liFreeKPart, myG &mapUpG)
{
    MAP_BASIC_G::iterator itG;
    MAP_BASIC_G::iterator itDegG;

    MAP_BASIC_G mapCalG;

    list<int> liNewPartG;
    HLST_DT_E hlstDtE;
    LT_PB_G::iterator itltPbI;
    list< pair<int, int> >::iterator itDegI;
    list< pair<int, int> >::iterator itNewPartG;
    TPST_PB_NODE stTpNode;
    MAP_E_KEY fTpKey;
    int eid = 0;

	struct timeval tv;
	long lBeginLoopTime = 0;
	long lStartTime = 0;
	long lCurTime = 0;
	long lDelSubTime = 0;
	long lUpdSubTime = 0;
	long lAddLSubTime = 0;
	long lGetGSubTime = 0;
	long lCpMpSubTime = 0;

	int iDebugI = 0;

    //printf("DEBUG begin del one G\n");

    hlstDtE.clear();
    hlstDtE.setOrderOffset(getStructOffset(ST_DT_E, ldDtE), delEFuncKeyDtE);
    hlstDtE.setFindOffset(getStructOffset(ST_DT_E, iKeyByEid));

    /* k = 2 needn't calculate */
    if (0 == iSup)
    {
        map <MAP_E_KEY, list<int> > mpTpG;
        map <MAP_E_KEY, list<int> >::iterator itmpTpG;
        for (itG = mapG.m_mpBasicG.begin(); itG != mapG.m_mpBasicG.end(); itG++)
        {
            if (itG->first.first < itG->first.second)
            {
                fTpKey = delEFuncKeyDtE(itG->second.second);
                eid = itG->second.first;
                mpTpG[fTpKey].push_back(eid);
            }
        }
        for (itmpTpG = mpTpG.begin(); itmpTpG != mpTpG.end(); itmpTpG++)
        {
            stTpNode.fKey = itmpTpG->first;
            stTpNode.ltESet = itmpTpG->second;
            ltPbG.push_back(stTpNode);
            stTpNode.ltESet.clear();
        }
        mpTpG.clear();
        return 0;
    }


    liNewPartG = liFreeKPart;

    gettimeofday(&tv, NULL);
    lBeginLoopTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    while (!pltUpPbG->empty())
    {
        itltPbI = pltUpPbG->begin();

        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        //printf("DEBUG begin cal\n");
        calPek_partL(liNewPartG, mapG, mapUpG, mapCalG);
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lUpdSubTime += lCurTime - lStartTime;
        g_lBuildVTime += lCurTime - lStartTime;

        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        delEGetGFromList(mapCalG, liNewPartG, mapG);
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lCpMpSubTime += lCurTime - lStartTime;

        /*printf("Delete OneG: %d\n", iDebugI);
        iDebugI++;*/
        /* no error */
        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        //printf("DEBUG begin add\n");
        debug_assert(delEAddGToDel(iSup, hlstDtE, liNewPartG, mapG) == false );
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lAddLSubTime += lCurTime - lStartTime;
        g_lBuildSortTime += lCurTime - lStartTime;

        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        delE_subG(iSup, itltPbI->fKey, mapG, ltPbG, hlstDtE, mapCalG);
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lDelSubTime += lCurTime - lStartTime;

        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

        /* next up set */
        liNewPartG = itltPbI->ltESet;

        pltUpPbG->erase(itltPbI);
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lGetGSubTime += lCurTime - lStartTime;

    }

    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    //printf("DEBUG begin cal 2\n");
    calPek_partL(liNewPartG, mapG, mapUpG, mapCalG);
    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lUpdSubTime += lCurTime - lStartTime;
    g_lBuildVTime += lCurTime - lStartTime;


    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    delEGetGFromList(mapCalG, liNewPartG, mapG);
    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lCpMpSubTime += lCurTime - lStartTime;

    /* no error */
    //printf("DEBUG begin add 2\n");
    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    debug_assert(delEAddGToDel(iSup, hlstDtE, liNewPartG, mapG) == false );
    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lAddLSubTime += lCurTime - lStartTime;
    g_lBuildSortTime += lCurTime - lStartTime;

    // printf("DEBUG end add\n");
    /* last */
    //debug_assert(0 == mapUpG.size());

    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    /* 1 maximum */
    delE_subG(iSup, 1.1, mapG, ltPbG, hlstDtE, mapCalG);
    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lDelSubTime += lCurTime - lStartTime;
    // printf("delSub Time=%ld ms\n", lDelSubTime);
    // printf("updSub Time=%ld ms\n", lUpdSubTime);
    // printf("addLi Time=%ld ms\n", lAddLSubTime);
    // printf("getG Time=%ld ms\n", lGetGSubTime);
    // printf("cpM Time=%ld ms\n", lCpMpSubTime);
    // printf("loop Time=%ld ms\n", lCurTime - lBeginLoopTime);

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
int delE_partG(MP_DECOMP_G &mpDeG, char *szSaveFile, myG &oInitG)
{
    MP_DECOMP_G::reverse_iterator  itDeG;
    LT_PB_G ltPbG;
    LT_PB_G ltUpPbG;
    int iPrevK = 0;
    int iCurK = 0;
    myG mapG;
    myG mapPrevG;
    myG mapUpG;
	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lTotalTime = 0;
	long lInitTime = 0;
	long lAddTime = 0;
	long lSaveTotalTime = 0;

    mapG.beforeDataInit();
    mapUpG.beforeDataInit();
    mapPrevG.beforeDataInit();
    mapPrevG.init(oInitG);

    /*printf("DEBUG PARTG (%d, %d) eid: %d\n",
           oInitG.m_mpBasicG.begin()->first.first,
           oInitG.m_mpBasicG.begin()->first.second,
           oInitG.m_mpBasicG.begin()->second.first);
    printf("DEBUG PARTG (%d, %d) maxEid: %d\n",
           oInitG.m_szG[3753].paXY.first,
           oInitG.m_szG[3753].paXY.second,
           oInitG.m_iMaxEId);*/

    for (itDeG = mpDeG.rbegin(); itDeG != mpDeG.rend(); itDeG++)
    {
        iCurK = itDeG->first;
        if (iCurK < 3)
        {
            /* needn't calculate */
        }
        else
        {
            // printf("k: %d\n", iCurK);

            /* clear */
            gettimeofday(&tv, NULL);
            lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            /* save */
            mapPrevG.addBatch(itDeG->second, oInitG);

            gettimeofday(&tv, NULL);
            lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            lInitTime = lCurTime - lStartTime;
            // printf("init G(k=%d) Time=%ld ms\n", iCurK, lInitTime);

            lStartTime = lCurTime;

            mapG.copyMap(mapPrevG);

            gettimeofday(&tv, NULL);
            lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            lAddTime = lCurTime - lStartTime;
            // printf("add G(k=%d) Time=%ld ms\n", iCurK, lAddTime);

            /* order */
            debug_assert((iPrevK > iCurK) || (0 == iPrevK));
            iPrevK = iCurK;

            ltUpPbG.swap(ltPbG);
            ltPbG.clear();
        }

        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        delE_oneG(iCurK - 2, mapG, ltPbG, &ltUpPbG, itDeG->second, mapUpG);
        mapG.swap(mapUpG);
        mapG.destroy();
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lTotalTime = lCurTime - lStartTime;
        // printf("delete G(k=%d) Time=%ld ms\n", iCurK, lTotalTime);

        lStartTime = lCurTime;
        file_savePbG(iCurK, ltPbG, szSaveFile, oInitG);
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        lTotalTime = lCurTime - lStartTime;
        // printf("save delete G(k=%d) Time=%ld ms\n", iCurK, lTotalTime);
        lSaveTotalTime += lTotalTime;

    }

    mapUpG.destroy();
    // printf("DEBUG total %ld %ld %ld %ld ms\n", g_lBuildVTime, g_lBuildSortTime, g_lEReSortTime, g_lEUpdateTime);
    return lSaveTotalTime;
}
