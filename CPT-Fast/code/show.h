#pragma once
/***************
show something
****************/

int show_listEdgeBySup(HAST_EDGE_BY_SUP &lstFindEdgeBySup);

int show_mapG(MAP_G &mapG);
int show_listNeib();
int show_mapDeG(LIST_DECOMP_G lstDecompG);
int show_NeibP(int x, int y, MAP_G &mapG, map<int, list<int> > &maplstPNe);
