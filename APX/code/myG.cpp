/***************
my class G
****************/

#include "common.h"
#include "myG.h"

/*****************
input:
        myG &oInitG
description:
        copy map
        calculate stage
******************/
bool myG::copyMap(myG &oInitG)
{
    m_mpBasicG = oInitG.m_mpBasicG;

    m_ppaFindEdgeByEid = oInitG.m_ppaFindEdgeByEid;
    m_iMaxPId = oInitG.m_iMaxPId;
    m_iMaxEId = oInitG.m_iMaxEId;
    m_mpG = oInitG.m_mpG;

    if (NULL == m_piPNeCnt)
    {
        /* first time */
        m_piPNeCnt = (int *)malloc(m_iMaxPId * sizeof(int));
    }
    memset(m_piPNeCnt, 0, m_iMaxPId * sizeof(int));
    memcpy(m_piPNeCnt, oInitG.m_piPNeCnt, m_iMaxPId * sizeof(int));

    return true;
}
/*****************
input:
        myG &oInitG
description:
        copy map
        calculate stage
******************/
bool myG::destroy()
{
    m_mpBasicG.clear();

    /* ignore m_ppaFindEdgeByEid, one program only has one */
    m_ppaFindEdgeByEid = NULL;
    m_iMaxPId = 0;
    m_iMaxEId = 0;
    m_mpG.clear();

    free(m_piPNeCnt);
    m_piPNeCnt = NULL;

    return true;
}
/*****************
input:
        int x
        int y
        long double ldP
description:
        add new edge
        init stage
******************/
bool myG::add(int x, int y, long double ldP)
{
    /* u < v */
    int u = 0;
    int v = 0;
    TPST_MAP_BY_XY stDesNode;
    MAP_G::iterator itGNode;

    if (x < y)
    {
        u = x;
        v = y;
    }
    else
    {
        u = y;
        v = x;
    }

    itGNode = this->m_mpG.find(pair<int, int>(u, v));
    if (itGNode != this->m_mpG.end())
    {
        /* have joined, get greater P */
        itGNode->second.p = (itGNode->second.p > ldP) ? itGNode->second.p : ldP;
        return false;
    }
    this->m_iMaxEId++;
    if (this->m_iMaxPId < v)
    {
        this->m_iMaxPId = v;
    }

    stDesNode.bReadyDel = false;
    stDesNode.eid = this->m_iMaxEId;
    stDesNode.iSup = 0;
    stDesNode.p = ldP;
    stDesNode.vldPek.clear();
    this->m_mpG[pair<int, int>(u, v)] = stDesNode;
    this->m_mpBasicG[pair<int, int>(u, v)];
    this->m_mpBasicG[pair<int, int>(v, u)];


    return true;
}
/*****************
input:
        MAP_BASIC_G &mpBasicG
        myG &oInitG
description:
        add new edges
        calculate stage
******************/
bool myG::addBatch(MAP_BASIC_G &mpBasicG, myG &oInitG)
{
    MAP_BASIC_G::iterator itMpG;
    MAP_G::iterator itInitG;

    for (itMpG = mpBasicG.begin(); itMpG != mpBasicG.end(); itMpG++)
    {
        itInitG = oInitG.m_mpG.find(itMpG->first);
        if (itInitG == oInitG.m_mpG.end())
        {
            /* error */
            debug_assert(0);
        }
        if (this->m_mpG.find(itMpG->first) != this->m_mpG.end())
        {
            /* already have */
            continue;
        }
        this->m_piPNeCnt[itMpG->first.first]++;
        this->m_piPNeCnt[itMpG->first.second]++;

        this->m_mpG[itMpG->first] = itInitG->second;

        this->m_mpBasicG[itMpG->first];
        this->m_mpBasicG[pair<int, int>(itMpG->first.second, itMpG->first.first)];
    }

    return true;
}
/*****************
input:
        MAP_BASIC_G &mpBasicG
        myG &oInitG
description:
        add new edges
        calculate stage
******************/
bool myG::addBatch(list<pair<int, int> > &lstE, myG &oInitG)
{
    list<pair<int, int> >::iterator itMpG;
    MAP_G::iterator itInitG;

    for (itMpG = lstE.begin(); itMpG != lstE.end(); itMpG++)
    {
        itInitG = oInitG.m_mpG.find(*itMpG);
        if (itInitG == oInitG.m_mpG.end())
        {
            /* error */
            debug_assert(0);
        }
        if (this->m_mpG.find(*itMpG) != this->m_mpG.end())
        {
            /* already have */
            continue;
        }
        this->m_piPNeCnt[itMpG->first]++;
        this->m_piPNeCnt[itMpG->second]++;

        this->m_mpG[*itMpG] = itInitG->second;

        this->m_mpBasicG[*itMpG];
        this->m_mpBasicG[pair<int, int>(itMpG->second, itMpG->first)];
    }

    return true;
}
/*****************
input:
        MAP_BASIC_G &mpBasicG
        myG &oInitG
description:
        add new edges
        calculate stage
******************/
bool myG::addBatch(myG &oInitG, long double ldMiniP)
{
    MAP_G::iterator itMpG;
    MAP_G::iterator itInitG;

    for (itMpG = oInitG.m_mpG.begin(); itMpG != oInitG.m_mpG.end(); itMpG++)
    {
        if (this->m_mpG.find(itMpG->first) != this->m_mpG.end())
        {
            /* already have */
            continue;
        }
        if (itMpG->second.p < ldMiniP)
        {
            continue;
        }
        this->m_piPNeCnt[itMpG->first.first]++;
        this->m_piPNeCnt[itMpG->first.second]++;

        this->m_mpG[itMpG->first] = itMpG->second;

        this->m_mpBasicG[itMpG->first];
        this->m_mpBasicG[pair<int, int>(itMpG->first.second, itMpG->first.first)];
    }

    return true;
}
/*****************
input:
        myG &oInitG
description:
        init from initial G
        calculate stage
******************/
bool myG::init(myG &oInitG)
{
    vector <int>::iterator itI;

    this->m_iMaxPId = oInitG.m_iMaxPId;
    this->m_iMaxEId = oInitG.m_iMaxEId;
    this->m_ppaFindEdgeByEid = oInitG.m_ppaFindEdgeByEid;

    if (NULL == m_piPNeCnt)
    {
        /* first time */
        m_piPNeCnt = (int *)malloc(m_iMaxPId * sizeof(int));
    }
    memset(m_piPNeCnt, 0, m_iMaxPId * sizeof(int));

    this->m_mpG.clear();
    this->m_mpBasicG.clear();

    return true;
}

/*****************
input:
        none
description:
        init object
        init stage
******************/
bool myG::simpleInit()
{
    m_mpBasicG.clear();
    m_piPNeCnt = NULL;
    m_ppaFindEdgeByEid = NULL;
    m_iMaxPId = 0;
    m_iMaxEId = 0;
    m_mpG.clear();
    return true;
}
/*****************
input:
        none
description:
        init other
        init stage
******************/
bool myG::otherInit()
{
    int x = 0;
    int y = 0;
    int eid = 0;
    int iPidMax = 0;
    int i = 0;
    float fP = 0;

    TPST_MAP_BY_XY *pstTempMapE;
    list<int> lstTempNeb;

    MAP_G::iterator itI;

    map<int, list<int> > maplstPNe;

    /* greater than max */
    m_iMaxPId++;
    m_iMaxEId++;

    m_ppaFindEdgeByEid = new pair <int, int> [m_iMaxEId];
    m_piPNeCnt = (int *)malloc(m_iMaxPId * sizeof(int));
    memset(m_piPNeCnt, 0, m_iMaxPId * sizeof(int));

    for (itI = this->m_mpG.begin(); itI != this->m_mpG.end(); itI++)
    {
        x = itI->first.first;
        y = itI->first.second;

        pstTempMapE = &(itI->second);
        eid = pstTempMapE->eid;

        m_ppaFindEdgeByEid[eid] = pair <int, int>(x, y);
        if (maplstPNe.find(x) == maplstPNe.end())
        {
            /* can't find set new list */
            lstTempNeb.push_front(y);
            maplstPNe[x] = lstTempNeb;
            lstTempNeb.clear();
            m_piPNeCnt[x] = 1;
        }
        else
        {
            /* find get list and push */
            maplstPNe[x].push_front(y);
            m_piPNeCnt[x]++;
        }
        if (maplstPNe.find(y) == maplstPNe.end())
        {
            /* can't find set new list */
            lstTempNeb.push_front(x);
            maplstPNe[y] = lstTempNeb;
            lstTempNeb.clear();
            m_piPNeCnt[y] = 1;
        }
        else
        {
            /* find get list and push */
            maplstPNe[y].push_front(x);
            m_piPNeCnt[y]++;
        }

    }

    return 0;
}

/*****************
input:
        int x
        int y
description:
        delete edge
        calculate stage
******************/
bool myG::del(int x, int y)
{
    /* u < v */
    int u = 0;
    int v = 0;

    if (x < y)
    {
        u = x;
        v = y;
    }
    else
    {
        u = y;
        v = x;
    }

    m_mpG.erase(pair<int, int>(u, v));
    m_mpBasicG.erase(pair<int, int>(u, v));
    m_mpBasicG.erase(pair<int, int>(v, u));
    m_piPNeCnt[u]--;
    m_piPNeCnt[v]--;
    return true;
}

/*****************
input:
        int x
        int y
        list<int> &lstNeib
description:
        delete edge
        calculate stage
******************/
int myG::findNeb(int x, int y, list<int> &lstNeib)
{
    MAP_BASIC_G::iterator itLowB;
    MAP_BASIC_G::iterator itHighB;
    int iNebCnt = 0;
    /* nei(u) < nei(v) */
    int u = 0;
    int v = 0;
    int w = 0;

    if (m_piPNeCnt[x] < m_piPNeCnt[y])
    {
        u = x;
        v = y;
    }
    else
    {
        u = y;
        v = x;
    }

    debug_assert(lstNeib.empty());
    /* find u's neighbor , note u > 0 */
    itLowB = m_mpBasicG.upper_bound(pair<int, int>(u, 0));
    itHighB = m_mpBasicG.upper_bound(pair<int, int>(u + 1, 0));
    while (itLowB != itHighB)
    {

        w = itLowB->first.second;
        if (m_mpBasicG.find(pair<int, int>(v, w)) != m_mpBasicG.end())
        {
            lstNeib.push_back(w);
            iNebCnt++;
        }
        itLowB++;
    }
    return iNebCnt;
}
/*****************
input:
        int x
        int y
        list<int> &lstNeib
        MAP_BASIC_G &mapCalG
description:
        delete edge
        calculate stage
******************/
int myG::findNebE(int x, int y, map<pair<int, int>, int> &mpNeibE, MAP_BASIC_G &mapCalG)
{
    MAP_BASIC_G::iterator itLowB;
    MAP_BASIC_G::iterator itHighB;
    int iNebCnt = 0;
    /* nei(u) < nei(v) */
    int u = 0;
    int v = 0;
    int w = 0;

    if (m_piPNeCnt[x] < m_piPNeCnt[y])
    {
        u = x;
        v = y;
    }
    else
    {
        u = y;
        v = x;
    }

    debug_assert(mpNeibE.empty());
    /* find u's neighbor , note u > 0 */
    itLowB = this->m_mpBasicG.upper_bound(pair<int, int>(u, 0));
    itHighB = this->m_mpBasicG.upper_bound(pair<int, int>(u + 1, 0));
    while (itLowB != itHighB)
    {
        w = itLowB->first.second;
        if (this->m_mpBasicG.find(pair<int, int>(v, w)) != this->m_mpBasicG.end())
        {
            if (mapCalG.find(pair<int, int>(u, w)) != mapCalG.end())
            {
                mpNeibE[pair<int, int>(u, w)] = v;
                iNebCnt++;
            }
            if (mapCalG.find(pair<int, int>(v, w)) != mapCalG.end())
            {
                mpNeibE[pair<int, int>(v, w)] = u;
                iNebCnt++;
            }
        }
        itLowB++;
    }
    return iNebCnt;
}

/*****************
input:
        int x
        int y
description:
        find node
******************/
TPST_MAP_BY_XY *myG::findNode(int x, int y)
{
    /* u < v */
    int u = 0;
    int v = 0;


    if (x < y)
    {
        u = x;
        v = y;
    }
    else
    {
        u = y;
        v = x;
    }
    if (m_mpG.find(pair<int, int>(u, v)) == m_mpG.end())
    {
        return NULL;
    }
    return &(m_mpG[pair<int, int>(u, v)]);
}

/*****************
input:
        int x
        int y
description:
        find node
******************/
TPST_MAP_BY_XY *myG::findNode(int eid)
{
    pair<int, int> paE;
    MAP_G::iterator itMNode;
    paE = m_ppaFindEdgeByEid[eid];
    itMNode = m_mpG.find(paE);

    debug_assert(itMNode != m_mpG.end());
    return &(itMNode->second);
}













