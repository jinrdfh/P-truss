#pragma once
/***************
delete edge in mapG
****************/


//long double delE_getDtE(int iSup, vector <long double> &vfPek, long double p);

//int delE_oneGByOnce(int iSup, myG &mapG, MAP_BASIC_G &mapUpG, long double ldUpDtE);
int delE_getGFromVal(MAP_BASIC_G &mpUpG, int iUpK, map <int, MAP_PB_G> &mpAllPbG, long double ldR);

