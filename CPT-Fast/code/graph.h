#pragma once
/***************
graph structure define
****************/

/*typedef struct tpstEdgeNode
{
    int iSup;
    int eid;
}HAST_EDGE_NODE;
typedef myHsList<HAST_EDGE_NODE, int, int, int> HAST_EDGE_BY_SUP;

typedef list<pair<int, int> >  LI_XY_E;
typedef list<pair< pair<int, int>, int> >  LI_COM_E;

typedef struct tpstDecompG
{
    int k;
    LI_XY_E liSubG;
}TPST_DECOMP_G;

typedef map<int, LI_XY_E> MP_DECOMP_G;

typedef float MAP_E_KEY;

typedef map<MAP_E_KEY, list<pair<int, int> > > MAP_PB_G;*/
/* <x, y>, <eid, initP> */

typedef list<pair< pair<int, int>, int> >  LI_COM_E;

typedef float MAP_E_KEY;
typedef struct tpstPBNode
{
    MAP_E_KEY fKey;
    list<int> ltESet;
}TPST_PB_NODE;

typedef list< TPST_PB_NODE > LT_PB_G;


typedef map<int, list<int> > MP_DECOMP_G;
typedef struct tpstDeTByEID
{
    int eid;
    int iSup;
    list<int>::iterator itPos;
}TPST_DE_T_BY_EID;


typedef struct stDtE
{
    long double ldDtE;
    int iKeyByEid;
}ST_DT_E;

typedef myHsList<ST_DT_E, long double, MAP_E_KEY, int > HLST_DT_E;







