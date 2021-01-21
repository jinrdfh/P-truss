/***************
truss decomposition
****************/
#include <sys/time.h>
#include "common.h"
#include "myG.h"
#include "deTruss.h"
#include "calPek.h"

static HAST_EDGE_BY_SUP l_lstFindEdgeBySup;

/*****************
input:
        myG &oMpG
output:
        HAST_EDGE_BY_SUP_AND_DTE l_lstFindEdgeBySupAndDtE
description:
        fill l_lstFindEdgeBySupAndDtE
******************/
int deFindSup(myG &oMpG)
{
    int x = 0;
    int y = 0;
    list<int> lstNeb;
    TPST_MAP_BY_XY *pstTempEdge;
    MAP_G::iterator itG;
    HAST_EDGE_NODE stTempSup;

    memset(&stTempSup, 0, sizeof(HAST_EDGE_NODE));

    /* init */
    l_lstFindEdgeBySup.setOrderOffset(getStructOffset(HAST_EDGE_NODE, iSup), NULL);
    l_lstFindEdgeBySup.setFindOffset(getStructOffset(HAST_EDGE_NODE, eid));

    for (itG = oMpG.m_mpG.begin(); itG != oMpG.m_mpG.end(); itG++)
    {
		x = itG->first.first;
		y = itG->first.second;
		pstTempEdge = &(itG->second);

        stTempSup.eid = pstTempEdge->eid;
        /* real support */
        lstNeb.clear();
        stTempSup.iSup = oMpG.findNeb(x, y, lstNeb);
        pstTempEdge->iSup = stTempSup.iSup;
        l_lstFindEdgeBySup.insetInMp(stTempSup);
    }

    return 0;
}
/*****************
input:
        myG &mapInitG
        LIST_DECOMP_G &lstDeG
output:
        LIST_DECOMP_G &lstDeG
description:
        truss decomposition
******************/
int deTruss(myG &mapInitG, LIST_DECOMP_G &lstDeG)
{
    myG oTempG;
    TPST_DECOMP_G stDesG;
    int x = 0;
    int y = 0;
    int iPrevK = 0;
    int iCurK = 0;
    int iEid = 0;
    pair <int, int> paXY;

    HAST_EDGE_NODE stCuEdge;
    HAST_EDGE_NODE stNewEdge;
    list<int>::iterator itlstE;
    list<int> lstWaitEId;
    TPST_MAP_BY_XY *pstTempEdge;
    map<pair<int, int>, int> mpNebE;
    map<pair<int, int>, int>::iterator itNebE;
	MAP_BASIC_G mapCalG;

	oTempG.simpleInit();
    oTempG.copyMap(mapInitG);

    mapCalG = oTempG.m_mpBasicG;

    deFindSup(oTempG);

    while (!l_lstFindEdgeBySup.empty())
	{
	    stCuEdge = l_lstFindEdgeBySup.getTop();
        if (stCuEdge.iSup + 2 > iCurK)
        {
            /* new k */
            iCurK = stCuEdge.iSup + 2;
            if (0 == iPrevK)
            {
                /* init */
            }
            else
            {
                stDesG.k = iPrevK;
                lstDeG.insetByOrder(stDesG);
                /* clear, new level */
                stDesG.mapSubG.clear();
            }
            iPrevK = iCurK;
        }

        debug_assert(lstWaitEId.empty());
	    l_lstFindEdgeBySup.getTopBatch(lstWaitEId);
	    for (itlstE = lstWaitEId.begin(); itlstE != lstWaitEId.end(); itlstE++)
        {
            l_lstFindEdgeBySup.delInMp(*itlstE);
            paXY = oTempG.findByEid(*itlstE);
            deTruss_mapSub<MAP_BASIC_G>(mapCalG, paXY.first, paXY.second);
        }

        while (!lstWaitEId.empty())
        {
            iEid = lstWaitEId.front();
            lstWaitEId.pop_front();

            paXY = oTempG.findByEid(iEid);
            x = paXY.first;
            y = paXY.second;
            pstTempEdge = oTempG.findNode(x, y);
            debug_assert(NULL != pstTempEdge);

            mpNebE.clear();
            oTempG.findNebE(x, y, mpNebE, mapCalG);

            for (itNebE = mpNebE.begin(); itNebE != mpNebE.end(); itNebE++)
            {
                int iTempEid = 0;
                int iSup = 0;

                pstTempEdge = oTempG.findNode(itNebE->first.first, itNebE->first.second);
                iTempEid = pstTempEdge->eid;

                iSup = pstTempEdge->iSup;

                debug_assert(iSup + 2 > iCurK);

                stNewEdge.eid = iTempEid;
                /* size is sup + 1, now sup - 1 */
                pstTempEdge->iSup--;
                stNewEdge.iSup = pstTempEdge->iSup;
                if (stNewEdge.iSup + 2 > iCurK)
                {
                    l_lstFindEdgeBySup.insetInMp(stNewEdge);
                }
                else
                {
                    /* to be deleted */
                    l_lstFindEdgeBySup.delInMp(iTempEid);
                    deTruss_mapSub<MAP_BASIC_G>(mapCalG, itNebE->first.first, itNebE->first.second);
                    lstWaitEId.push_back(iTempEid);
                }
            }
            /* delete in sub graph */
            stDesG.mapSubG[pair<int, int>(x, y)];
            oTempG.del(x, y);
        }
	}

    if (!stDesG.mapSubG.empty())
    {
        /* last G */
        stDesG.k = iCurK;
        lstDeG.insetByOrder(stDesG);
    }

    oTempG.destroy();
    return 0;
}
