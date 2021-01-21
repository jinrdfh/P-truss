#pragma once
/***************
delete edge in mapG
****************/

int delE_partG(MP_DECOMP_G &mpDeG, char *szSaveFile, myG &oInitG);

long double delE_getDtE(int iSup, vector <long double> &vfPek, long double p, int iVLen);

