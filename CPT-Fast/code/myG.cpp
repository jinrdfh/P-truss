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
    /*printf("DEBUG COPYMAP (%d, %d) eid: %d oEid: %d\n",
           m_mpBasicG.begin()->first.first,
           m_mpBasicG.begin()->first.second,
           m_mpBasicG.begin()->second.first,
           oInitG.m_mpBasicG.begin()->second.first);*/

    m_iMaxPId = oInitG.m_iMaxPId;
    m_iMaxEId = oInitG.m_iMaxEId;

    /* first time */
    debug_assert(NULL == m_piPNeCnt);
    m_piPNeCnt = (int *)malloc(m_iMaxPId * sizeof(int));
    debug_assert(NULL != m_piPNeCnt);
    memset(m_piPNeCnt, 0, m_iMaxPId * sizeof(int));
    /* no need to copy, set to 0 */
    // memcpy(m_piPNeCnt, oInitG.m_piPNeCnt, m_iMaxPId * sizeof(int));

    /* first time */
    debug_assert(NULL == m_szG);
    m_szG = (TPST_MAP_BY_XY *)malloc(m_iMaxEId * sizeof(TPST_MAP_BY_XY));
    debug_assert(NULL != m_szG);
    memset(m_szG, 0, m_iMaxEId * sizeof(TPST_MAP_BY_XY));
    memcpy(m_szG, oInitG.m_szG, m_iMaxEId * sizeof(TPST_MAP_BY_XY));
    return true;
}

/*****************
input:
        myG &oInitG
description:
        copy map
        calculate stage
******************/
bool myG::swap(myG &oDesG)
{
    int *piTempPNeCnt = NULL;
    TPST_MAP_BY_XY *szTempG = NULL;
    m_mpBasicG.swap(oDesG.m_mpBasicG);

    oDesG.m_iMaxPId = m_iMaxPId;
    oDesG.m_iMaxEId = m_iMaxEId;

    piTempPNeCnt = oDesG.m_piPNeCnt;
    oDesG.m_piPNeCnt = m_piPNeCnt;
    m_piPNeCnt = piTempPNeCnt;

    szTempG = oDesG.m_szG;
    oDesG.m_szG = m_szG;
    m_szG = szTempG;

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

    m_iMaxPId = 0;
    m_iMaxEId = 0;

    free(m_piPNeCnt);
    m_piPNeCnt = NULL;

    free(m_szG);
    m_szG = NULL;

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
    MAP_BASIC_G::iterator itGNode;
    long double ldCurP = 0;

    itGNode = m_mpBasicG.find(pair<int, int>(x, y));
    if (itGNode != m_mpBasicG.end())
    {
        /* have joined, get greater P */
        ldCurP = itGNode->second.second;
        ldCurP = (ldCurP > ldP) ? ldCurP : ldP;
        itGNode->second.second = ldCurP;
        m_mpBasicG[pair<int, int>(y, x)].second = ldCurP;
        return false;
    }
    m_iMaxEId++;
    if (m_iMaxPId < x)
    {
        m_iMaxPId = x;
    }
    if (m_iMaxPId < y)
    {
        m_iMaxPId = y;
    }

    m_mpBasicG[pair<int, int>(x, y)] = pair<int, long double>(m_iMaxEId, ldP);
    m_mpBasicG[pair<int, int>(y, x)] = pair<int, long double>(m_iMaxEId, ldP);

    return true;
}
#if 0
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
    debug_assert(0);
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
#endif
/*****************
input:
        list<int> &lstE
        myG &oInitG
description:
        add new edges
        calculate stage
******************/
bool myG::addBatch(list<int> &lstE, myG &oInitG)
{
    list<int>::iterator itE;
    pair<int, int> paXY;

    for (itE = lstE.begin(); itE != lstE.end(); itE++)
    {
        paXY = oInitG.m_szG[*itE].paXY;

        if (m_mpBasicG.find(paXY) != m_mpBasicG.end())
        {
            /* already have */
            continue;
        }
        m_piPNeCnt[paXY.first]++;
        m_piPNeCnt[paXY.second]++;

        m_mpBasicG[paXY] = oInitG.m_mpBasicG[paXY];
        m_mpBasicG[pair<int, int>(paXY.second, paXY.first)] = oInitG.m_mpBasicG[pair<int, int>(paXY.second, paXY.first)];
        /*printf("DEBUG ADDBATCH (%d, %d) eid: %d oEid: %d\n",
               paXY.first,
               paXY.second,
               m_mpBasicG[paXY].first,
               oInitG.m_mpBasicG[paXY].first);*/
    }

    return true;
}
#if 0
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
    debug_assert(0);
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
#endif
/*****************
input:
        myG &oInitG
description:
        init from initial G
        calculate stage
******************/
bool myG::init(myG &oInitG)
{
    this->m_iMaxPId = oInitG.m_iMaxPId;
    this->m_iMaxEId = oInitG.m_iMaxEId;

    /* first time */
    debug_assert(NULL == m_piPNeCnt);
    m_piPNeCnt = (int *)malloc(m_iMaxPId * sizeof(int));
    debug_assert(NULL != m_piPNeCnt);
    memset(m_piPNeCnt, 0, m_iMaxPId * sizeof(int));
    memcpy(m_piPNeCnt, oInitG.m_piPNeCnt, m_iMaxPId * sizeof(int));

    /* first time */
    debug_assert(NULL == m_szG);
    m_szG = (TPST_MAP_BY_XY *)malloc(m_iMaxEId * sizeof(TPST_MAP_BY_XY));
    debug_assert(NULL != m_szG);
    memset(m_szG, 0, m_iMaxEId * sizeof(TPST_MAP_BY_XY));
    memcpy(m_szG, oInitG.m_szG, m_iMaxEId * sizeof(TPST_MAP_BY_XY));

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
bool myG::beforeDataInit()
{
    m_mpBasicG.clear();
    m_piPNeCnt = NULL;
    m_szG = NULL;
    m_iMaxPId = 0;
    m_iMaxEId = 0;
    return true;
}
/*****************
input:
        none
description:
        init after data input
        init stage
******************/
bool myG::afterDataInit()
{
    int x = 0;
    int y = 0;
    int eid = 0;
    //int iDebugSize = 0;
    long double fP = 0;
    vector <long double> vldPek;

    MAP_BASIC_G::iterator itI;

    /* greater than max */
    m_iMaxPId++;
    m_iMaxEId++;

    m_piPNeCnt = (int *)malloc(m_iMaxPId * sizeof(int));
    debug_assert(NULL != m_piPNeCnt);
    memset(m_piPNeCnt, 0, m_iMaxPId * sizeof(int));

    m_szG = (TPST_MAP_BY_XY *)malloc(m_iMaxEId * sizeof(TPST_MAP_BY_XY));
    debug_assert(NULL != m_szG);
    memset(m_szG, 0, m_iMaxEId * sizeof(TPST_MAP_BY_XY));
    /*iDebugSize = m_iMaxEId * sizeof(TPST_MAP_BY_XY);
    printf("DEBUG malloc size: %d\n", iDebugSize);
    debug_assert(0);*/

    vldPek.clear();

    for (itI = m_mpBasicG.begin(); itI != m_mpBasicG.end(); itI++)
    {
        x = itI->first.first;
        y = itI->first.second;

        if (x >= y)
        {
            continue;
        }

        eid = itI->second.first;
        fP = itI->second.second;

        m_piPNeCnt[x]++;

        m_piPNeCnt[y]++;

        m_szG[eid].eid = eid;
        m_szG[eid].iSup = 0;
        m_szG[eid].iVPLen = 0;
        m_szG[eid].bReadyDel = false;
        m_szG[eid].p = fP;
        m_szG[eid].paXY = itI->first;
        m_szG[eid].vldPek = vldPek;

        m_szG[eid].iTriggerEid = 0;
        m_szG[eid].vldTriggerPek = vldPek;
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
    m_mpBasicG.erase(pair<int, int>(x, y));
    m_mpBasicG.erase(pair<int, int>(y, x));
    m_piPNeCnt[x]--;
    m_piPNeCnt[y]--;
    return true;
}

/*****************
input:
        int eid
description:
        delete edge
        calculate stage
******************/
bool myG::del(int eid)
{
    int x = 0;
    int y = 0;
    pair<int, int> paXY;
    vector <long double> vldTpV;

    paXY = findByEid(eid);
    x = paXY.first;
    y = paXY.second;

    m_mpBasicG.erase(pair<int, int>(x, y));
    m_mpBasicG.erase(pair<int, int>(y, x));
    m_piPNeCnt[x]--;
    m_piPNeCnt[y]--;
    vldTpV.clear();
    m_szG[eid].vldPek.swap(vldTpV);
    vldTpV.clear();
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
        int eid
        list<int> &lstNeib
description:
        delete edge
        calculate stage
******************/
int myG::findNeb(int eid, list<int> &lstNeib)
{
    int iNebCnt = 0;
    pair<int, int> paXY;

    paXY = m_szG[eid].paXY;

    iNebCnt = findNeb(paXY.first, paXY.second, lstNeib);

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
int myG::findNeb(int x, int y, list<int> &lstNeib, MAP_BASIC_G &mapCalG)
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
    itLowB = mapCalG.upper_bound(pair<int, int>(v, 0));
    itHighB = mapCalG.upper_bound(pair<int, int>(v + 1, 0));
    while (itLowB != itHighB)
    {
        w = itLowB->first.second;
        if (this->m_mpBasicG.find(pair<int, int>(u, w)) != this->m_mpBasicG.end())
        {
            lstNeib.push_back(w);
            iNebCnt++;
        }
        itLowB++;
    }
    itLowB = mapCalG.upper_bound(pair<int, int>(u, 0));
    itHighB = mapCalG.upper_bound(pair<int, int>(u + 1, 0));
    while (itLowB != itHighB)
    {
        w = itLowB->first.second;
        if (this->m_mpBasicG.find(pair<int, int>(v, w)) != this->m_mpBasicG.end())
        {
            if (mapCalG.find(pair<int, int>(v, w)) == mapCalG.end())
            {
                /* haven't been recorded */
                lstNeib.push_back(w);
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
        LI_COM_E &liNeibE
        MAP_BASIC_G &mapCalG
description:
        delete edge
        calculate stage
******************/
int myG::findNebE(int x, int y, LI_COM_E &liNeibE, MAP_BASIC_G &mapCalG)
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

    debug_assert(liNeibE.empty());
    /* find u's neighbor , note u > 0 */
    itLowB = mapCalG.upper_bound(pair<int, int>(u, 0));
    itHighB = mapCalG.upper_bound(pair<int, int>(u + 1, 0));
    while (itLowB != itHighB)
    {
        w = itLowB->first.second;
        if (this->m_mpBasicG.find(pair<int, int>(v, w)) != this->m_mpBasicG.end())
        {
            liNeibE.push_back(pair< pair<int, int>, int> (pair<int, int>(u, w), v));
            iNebCnt++;
        }
        itLowB++;
    }
    itLowB = mapCalG.upper_bound(pair<int, int>(v, 0));
    itHighB = mapCalG.upper_bound(pair<int, int>(v + 1, 0));
    while (itLowB != itHighB)
    {
        w = itLowB->first.second;
        if (this->m_mpBasicG.find(pair<int, int>(u, w)) != this->m_mpBasicG.end())
        {
            //mpNeibE[pair<int, int>(v, w)] = u;
            liNeibE.push_back(pair< pair<int, int>, int> (pair<int, int>(v, w), u));
            iNebCnt++;
        }
        itLowB++;
    }
    return iNebCnt;
}

/*****************
input:
        int eid
        LI_COM_E &liNeibE
        MAP_BASIC_G &mapCalG
description:
        delete edge
        calculate stage
******************/
int myG::findNebE(int eid, LI_COM_E &liNeibE, MAP_BASIC_G &mapCalG)
{
    int iNebCnt = 0;
    pair<int, int> paXY;

    paXY = m_szG[eid].paXY;

    iNebCnt = findNebE(paXY.first, paXY.second, liNeibE, mapCalG);

    return iNebCnt;
}

/*****************
input:
        int eid
        LI_COM_E &liNeibE
        MAP_BASIC_G &mapCalG
description:
        delete edge
        calculate stage
******************/
int myG::findNebE(int eid, list<int> &liNeibE)
{
    int iNebCnt = 0;
    int x = 0;
    int y = 0;
    pair<int, int> paXY;

    MAP_BASIC_G::iterator itLowB;
    MAP_BASIC_G::iterator itHighB;
    /* nei(u) < nei(v) */
    int u = 0;
    int v = 0;
    int w = 0;

    paXY = m_szG[eid].paXY;
    x = paXY.first;
    y = paXY.second;

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

    debug_assert(liNeibE.empty());
    /* find u's neighbor , note u > 0 */
    itLowB = m_mpBasicG.upper_bound(pair<int, int>(u, 0));
    itHighB = m_mpBasicG.upper_bound(pair<int, int>(u + 1, 0));
    while (itLowB != itHighB)
    {
        w = itLowB->first.second;
        if (m_mpBasicG.find(pair<int, int>(v, w)) != m_mpBasicG.end())
        {
            liNeibE.push_back(m_mpBasicG[pair<int, int>(u, w)].first);
            liNeibE.push_back(m_mpBasicG[pair<int, int>(v, w)].first);
            iNebCnt += 2;
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
    int eid = 0;
    if (m_mpBasicG.find(pair<int, int>(x, y)) == m_mpBasicG.end())
    {
        debug_assert(0);
        return NULL;
    }
    eid = m_mpBasicG[pair<int, int>(x, y)].first;
    // printf("DEBUG eid: %d\n", eid);
    return findNode(eid);
}

/*****************
input:
        int eid
description:
        find node
******************/
TPST_MAP_BY_XY *myG::findNode(int eid)
{
    // printf("DEBUG eid: %d p0: %p pe: %p\n", eid, m_szG, m_szG + eid);
    return m_szG + eid;
}













