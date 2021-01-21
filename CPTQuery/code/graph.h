#pragma once
/***************
graph structure define
****************/

typedef struct tpstEdgeNode
{
    int iSup;
    int eid;
}HAST_EDGE_NODE;
typedef myHsList<HAST_EDGE_NODE, int, int> HAST_EDGE_BY_SUP;

typedef map<long double, list<pair<int, int> > > MAP_PB_G;

typedef struct stDtE
{
    long double ldDtE;
    int iKeyByEid;
}ST_DT_E;

typedef myHsList<ST_DT_E, long double, int > HLST_DT_E;







