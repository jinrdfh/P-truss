#pragma once
/***************
input file output file function
****************/

#define DATA_PATH ("../data/test.txt")
#define TRUSS_FILE ("truss.txt")
#define ONE_LINE_BUFFER 100
#define FILE_NAME_BUFFER 100
/* read file and fill the list g_lstFindEdgeByP */
int file_fillG(char *szFileName, long double ldMinP, myG &mpG);

int file_savePbG(int k, MAP_PB_G &mapPbG, char *szSaveFile);
int file_saveDeTruss(LIST_DECOMP_G &lstDeG, char *szTrussPath);
