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
        map<int, list<int> > &maplstPNe
        MAP_G &mapInitG         fill sup
        int iSup
        list<int> &lstWaitDel
output:
        HAST_EDGE_BY_SUP l_lstFindEdgeBySup
description:
        fill l_lstFindEdgeBySup
******************/
int de_FindSupBySup(myG &mapG, int iSup, list<int> &lstWaitDel)
{
    int x = 0;
    int y = 0;
    int iEidSup = 0;
    MAP_G::iterator itmpG;
    list<int> lstNeb;

    for (itmpG = mapG.m_mpG.begin(); itmpG != mapG.m_mpG.end(); itmpG++)
    {
        x = itmpG->first.first;
        y = itmpG->first.second;
        lstNeb.clear();

        iEidSup = mapG.findNeb(x, y, lstNeb);

        itmpG->second.iSup = iEidSup;
        if (iEidSup < iSup)
        {
            lstWaitDel.push_back(itmpG->second.eid);
        }
    }

    return 0;
}

/*****************
input:
        int *piPNeCnt
        MAP_G &mapG
        int k
output:
        int k
description:
        truss decomposition by k
******************/
int deTruss_byK(myG &mapG, int k)
{
    int x = 0;
    int y = 0;
    int iEid = 0;
    pair <int, int> paXY;
    TPST_MAP_BY_XY *pstNode;

    list<int> lstNeb;
    list<int> lstWaitDel;
    list<int>::iterator itNeb;
    list<int>::iterator itCurDel;

    if (2 >= k)
    {
        /* needn't calculate */
        return 0;
    }

    de_FindSupBySup(mapG, k - 2, lstWaitDel);
    while (!lstWaitDel.empty())
	{
	    itCurDel = lstWaitDel.begin();
	    iEid = *itCurDel;
	    lstWaitDel.pop_front();

	    paXY = mapG.findByEid(iEid);
	    x = paXY.first;
	    y = paXY.second;

        lstNeb.clear();
        mapG.findNeb(x, y, lstNeb);
        for (itNeb = lstNeb.begin(); itNeb != lstNeb.end(); itNeb++)
        {
            int w = *itNeb;
            int iTempEid = 0;
            int iSup = 0;

            pstNode = mapG.findNode(x, w);
            debug_assert(NULL != pstNode);

            iTempEid = pstNode->eid;
            iSup = pstNode->iSup;

            if (iSup + 2 >= k)
            {
                pstNode->iSup = iSup - 1;
                if (iSup + 2 == k)
                {
                    lstWaitDel.push_back(iTempEid);
                }
            }
            pstNode = mapG.findNode(y, w);

            iTempEid = pstNode->eid;
            iSup = pstNode->iSup;

            if (iSup + 2 >= k)
            {
                pstNode->iSup = iSup - 1;
                if (iSup + 2 == k)
                {
                    lstWaitDel.push_back(iTempEid);
                }
            }
        }
        /* delete in sub graph */
        mapG.del(x, y);
	}

    return 0;
}
