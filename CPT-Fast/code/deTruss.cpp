/***************
truss decomposition
****************/
#include <sys/time.h>
#include "common.h"
#include "myG.h"
#include "deTruss.h"

static TPST_DE_T_BY_EID *l_szAllE;
/*****************
input:
        myG &oMpG
output:
        HAST_EDGE_BY_SUP_AND_DTE l_lstFindEdgeBySupAndDtE
description:
        fill l_lstFindEdgeBySupAndDtE
******************/
int deFindSup(myG &oMpG, MP_DECOMP_G &mpDeG)
{
    int iComNebCnt = 0;
    int iDebugCntMax = 0;
    list<int> lstNeb;
    int eid = 0;
    for (eid = 1; eid < oMpG.m_iMaxEId; eid++)
    {
        if (oMpG.m_szG[eid].eid != eid)
        {
            /* not exist */
            continue;
        }
        lstNeb.clear();
        iComNebCnt = oMpG.findNeb(eid, lstNeb);
        /* k */
        mpDeG[iComNebCnt + 2].push_front(eid);
        l_szAllE[eid].eid = eid;
        l_szAllE[eid].iSup = iComNebCnt;
        l_szAllE[eid].itPos = mpDeG[iComNebCnt + 2].begin();

        if (iDebugCntMax < iComNebCnt)
        {
            iDebugCntMax = iComNebCnt;
        }

    }
    // printf("DEBUG cnt max: %d\n", iDebugCntMax);

    return 0;
}
/*****************
input:
        myG &oMpG
        list<int> &delL
        map<int, int> &mpAffectedCnt
output:
        MP_DECOMP_G &mpDeG
description:
        truss decomposition
******************/
int reInsertNeib(myG &oMpG, int eid, int iSup, MP_DECOMP_G &mpDeG)
{
    int iCurId = 0;
    int iCurSup = 0;
    list<int> lstNebE;
    list<int>::iterator itNebE;
    lstNebE.clear();
    oMpG.findNebE(eid, lstNebE);

    for (itNebE = lstNebE.begin(); itNebE != lstNebE.end(); itNebE++)
    {
        iCurSup = l_szAllE[*itNebE].iSup;
        if (iCurSup <= iSup)
        {
            /* handling or have handled, ignore */
            continue;
        }
        iCurSup--;
        l_szAllE[*itNebE].iSup = iCurSup;
        if (iCurSup == iSup)
        {
            /* fall in back */
            mpDeG[iCurSup + 2].push_back(*itNebE);
            /* remove */
            mpDeG[iCurSup + 3].erase(l_szAllE[*itNebE].itPos);
            l_szAllE[*itNebE].itPos = mpDeG[iCurSup + 2].end();
        }
        else
        {
            /* fall in front, and record position */
            mpDeG[iCurSup + 2].push_front(*itNebE);
            mpDeG[iCurSup + 3].erase(l_szAllE[*itNebE].itPos);
            l_szAllE[*itNebE].itPos = mpDeG[iCurSup + 2].begin();
        }
    }
    return 0;
}
/*****************
input:
        myG &mapInitG
        MP_DECOMP_G &mpDeG
output:
        MP_DECOMP_G &mpDeG
description:
        truss decomposition
******************/
int deTruss(myG &mapInitG, MP_DECOMP_G &mpDeG)
{
    int iCurK = 0;
    int iCurE = 0;
    MP_DECOMP_G::iterator itDecG;
    MP_DECOMP_G::iterator itNextNode;
    list<int>::iterator itCurE;
    myG mapTpG;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lFindTime = 0;
	long lDeTime = 0;
	long lClearTime = 0;

    mapTpG.beforeDataInit();
    mapTpG.copyMap(mapInitG);

    l_szAllE = (TPST_DE_T_BY_EID *)malloc(mapTpG.m_iMaxEId * sizeof(TPST_DE_T_BY_EID));
    debug_assert(NULL != l_szAllE);
    memset(l_szAllE, 0, mapTpG.m_iMaxEId * sizeof(TPST_DE_T_BY_EID));

    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    deFindSup(mapTpG, mpDeG);
    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lFindTime += lCurTime - lStartTime;

    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    for (itDecG = mpDeG.begin(); itDecG != mpDeG.end(); itDecG++)
    {
        iCurK = itDecG->first;
        //printf("debug curK: %d\n", iCurK);
        for (itCurE = itDecG->second.begin(); itCurE != itDecG->second.end(); itCurE++)
        {
            iCurE = *itCurE;
            /*printf("curE (%d, %d)\n",
                   mapTpG.m_szG[iCurE].paXY.first,
                   mapTpG.m_szG[iCurE].paXY.second);*/
            reInsertNeib(mapTpG, iCurE, iCurK - 2, mpDeG);
            mapTpG.del(iCurE);
        }
    }
    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lDeTime += lCurTime - lStartTime;


    gettimeofday(&tv, NULL);
    lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    /* delete empty */
    itDecG = mpDeG.begin();
    while (itDecG != mpDeG.end())
    {
        itNextNode = itDecG;
        ++itNextNode;
        if (itNextNode == mpDeG.end())
        {
            break;
        }

        if (itNextNode->second.empty())
        {
            mpDeG.erase(itNextNode);
        }
        else
        {
            itDecG = itNextNode;
        }
    }

    itDecG = mpDeG.begin();
    if ((!mpDeG.empty()) && itDecG->second.empty())
    {
        mpDeG.erase(itDecG);
    }

    /*printf("DEBUG PARTG (%d, %d) eid: %d size: %d\n",
           mapTpG.m_mpBasicG.begin()->first.first,
           mapTpG.m_mpBasicG.begin()->first.second,
           mapTpG.m_mpBasicG.begin()->second.first,
           mapTpG.m_mpBasicG.size());*/
    mapTpG.destroy();
    gettimeofday(&tv, NULL);
    lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    lClearTime += lCurTime - lStartTime;

    // printf("DEBUG find time: %ld ms De time: %ld ms Clear time: %ld ms\n",
    //        lFindTime, lDeTime, lClearTime);

    return 0;
}
