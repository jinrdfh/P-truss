#pragma once
/***************
my class G
****************/

//typedef map<pair<int, int>, TPST_MAP_BY_XY> MAP_G;

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

typedef map<pair<int, int>, pair<int, long double> > MAP_BASIC_G;

typedef struct tpstMapByXY
{
    int eid;
    int iSup;
    int iVPLen;
    int iVTgLen;
    int iTriggerEid;
    bool bReadyDel;
    long double p;
    pair<int, int> paXY;
    vector <long double> vldPek;
    vector <long double> vldTriggerPek;
}TPST_MAP_BY_XY;

class myG
{
public:
    /* double */
    MAP_BASIC_G m_mpBasicG;
    int *m_piPNeCnt;
    // pair <int, int> *m_ppaFindEdgeByEid;

    int m_iMaxPId;
    int m_iMaxEId;
    /* single, x < y */
    // MAP_G m_mpG;
    TPST_MAP_BY_XY *m_szG;

    bool add(int x, int y, long double ldP);
    bool beforeDataInit();
    bool afterDataInit();
    bool copyMap(myG &oInitG);
    bool swap(myG &oDesG);
    int findNeb(int x, int y, list<int> &lstNeib);
    int findNeb(int eid, list<int> &lstNeib);
    int findNeb(int x, int y, list<int> &lstNeib, MAP_BASIC_G &mapCalG);
    int findNebE(int x, int y, LI_COM_E &liNeibE, MAP_BASIC_G &mapCalG);
    int findNebE(int eid, LI_COM_E &liNeibE, MAP_BASIC_G &mapCalG);
    int findNebE(int eid, list<int> &liNeibE);
    bool destroy();
    bool del(int x, int y);
    bool del(int eid);
    /* will not copy map, will copy array */
    bool init(myG &oInitG);
    bool addBatch(list<int> &lstE, myG &oInitG);
    TPST_MAP_BY_XY *findNode(int eid);
    TPST_MAP_BY_XY *findNode(int x, int y);

    pair <int, int> findByEid(int eid)
    {
        return m_szG[eid].paXY;
    }
    /* ??? */
    // bool addBatch(MAP_BASIC_G &mpBasicG, myG &oInitG);
    // bool addBatch(list<pair<int, int> > &lstE, myG &oInitG);
    // bool addBatch(myG &oInitG, long double ldMiniP);

};
