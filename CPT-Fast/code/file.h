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

int file_savePbG(int k, LT_PB_G &ltPbG, char *szSaveFile, myG &mpG);
int file_saveDeTruss(MP_DECOMP_G &mpDeG, char *szTrussPath, myG &mpG);
