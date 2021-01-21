#pragma once
/***************
my class G
****************/

typedef map<pair<int, int>, bool> MAP_BASIC_G;

typedef struct tpstDecompG
{
    int k;
    MAP_BASIC_G mapSubG;
}TPST_DECOMP_G;

typedef myList<TPST_DECOMP_G, int> LIST_DECOMP_G;

typedef struct tpstMapByXY
{
    int eid;
    long double p;
    int iSup;
    bool bReadyDel;
    vector <long double> vldPek;
}TPST_MAP_BY_XY;
typedef map<pair<int, int>, TPST_MAP_BY_XY> MAP_G;

/* base = init - sub */
template <typename mpTA, typename mpTB>
bool myG_mapSub(mpTA &mpBase, mpTB &mpInitG, mpTA &mpSub)
{
    typename mpTB::iterator itmapT;

    for (itmapT = mpInitG.begin(); itmapT != mpInitG.end(); itmapT++)
    {
        if (mpSub.find(itmapT->first) == mpSub.end())
        {
            /* doesn't have */
            mpBase[itmapT->first];
        }
    }
    return 0;
}

#define myG_getPair(iX, iY) ((iX < iY) ? (pair<int, int>(iX, iY)) : (pair<int, int>(iY, iX)))

class myG
{
public:
    /* double */
    MAP_BASIC_G m_mpBasicG;
    int *m_piPNeCnt;
    pair <int, int> *m_ppaFindEdgeByEid;

    int m_iMaxPId;
    int m_iMaxEId;
    /* single, x < y */
    MAP_G m_mpG;

    bool add(int x, int y, long double ldP);
    bool simpleInit();
    bool otherInit();
    bool copyMap(myG &oInitG);
    bool del(int x, int y);
    int findNeb(int x, int y, list<int> &lstNeib);
    int findNebE(int x, int y, map<pair<int, int>, int> &mpNeibE, MAP_BASIC_G &mapCalG);
    bool addBatch(MAP_BASIC_G &mpBasicG, myG &oInitG);
    bool addBatch(list<pair<int, int> > &lstE, myG &oInitG);
    bool addBatch(myG &oInitG, long double ldMiniP);
    bool init(myG &oInitG);
    bool destroy();

    pair <int, int> findByEid(int eid)
    {
        return m_ppaFindEdgeByEid[eid];
    }
    TPST_MAP_BY_XY *findNode(int x, int y);
    TPST_MAP_BY_XY *findNode(int eid);
};
