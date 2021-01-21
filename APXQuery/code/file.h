#pragma once
/***************
input file output file function
****************/

#define DATA_PATH ("../data/test.txt")
#define TRUSS_FILE ("truss.index")
#define ONE_LINE_BUFFER 100
#define FILE_NAME_BUFFER 100
/* read file and fill the list g_lstFindEdgeByP */

int file_fillG(char *szFileName, long double ldMinP, myG &mpG);


int file_getAllPbG(map <int, MAP_PB_G> &mpAllPbG, char *szDesPath, int iKMax);
int file_readDeTruss(LIST_DECOMP_G &lstDeG, char *szTrussPath, map <int, bool> &mpNK);

int file_readQuery(list<pair<int, float> > &lstQuery, char *szSaveFile);
int file_saveOneRes(int iDesK, float fDesR, MAP_BASIC_G &mpRes, char *szResPath, char *szLabel);
