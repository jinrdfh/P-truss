/***************
get graph
second step
input k and r
main cpp
****************/
#include <sys/time.h>
#include "common.h"

#define FILE_NAME_BUFFER 100
#define ONE_LINE_BUFFER 100
/*****************
input:
        int k
        long double ldR
        char *szDataCatalogName
        char *szSaveFileName
description:
        main function
******************/
int getGraph(int k, long double ldR, char *szDataCatalogName, char *szSaveFileName)
{
    char szDataFileName[FILE_NAME_BUFFER]= {0};
    char szLineBuffer[ONE_LINE_BUFFER]= {0};
    FILE *fpData = NULL;
    FILE *fpResult = NULL;
    int iEidX = 0;
    int iEidY = 0;
    long double ldCurR = 0;
    bool bWriteFlag = false;
    /* get data file name */
    sprintf(szDataFileName, "%s/%d-truss.txt", szDataCatalogName, k);
    fpData = fopen(szDataFileName, "rt");
    if (NULL == fpData)
    {
        printf("open data file error: %s\n", szDataFileName);
        debug_assert(0);
    }
    fpResult = fopen(szSaveFileName, "wt");
    if (NULL == fpResult)
    {
        printf("open result file error\n");
        debug_assert(0);
    }
    /* read file */
    while (fgets(szLineBuffer, ONE_LINE_BUFFER, fpData) != NULL)
    {
        /* decode */
        if (sscanf(szLineBuffer, "r,%Le", &ldCurR) == 1)
        {
            /* read probability */
            if (ldCurR >= ldR)
            {
                bWriteFlag = true;
            }
        }
        else if (sscanf(szLineBuffer, "%d,%d", &iEidX, &iEidY) == 2)
        {
            /* read edge id */
            if (bWriteFlag)
            {
                /* write data */
                fprintf(fpResult, "%d,%d\n", iEidX, iEidY);
            }
        }
        else
        {
            printf("decode error\n");
            debug_assert(0);
        }
    }
    fclose(fpData);
    fpData = NULL;
    fclose(fpResult);
    fpResult = NULL;
    return 0;
}
/*****************
input:
        k           truss
        r           probability
        input data catalog name
        output result file name
description:
        main function
******************/
int main(int argc, char *argv[])
{
    int k = 0;
    long double ldR = 0;
	struct timeval tv;
	long lStartTime = 0;
	long lTotalTime = 0;

    if (5 != argc)
    {
        printf("argc: %d\n", argc);
        debug_assert(0);
    }
    debug_assert(NULL != argv[3]);
    debug_assert(NULL != argv[4]);

    sscanf(argv[1], "%d", &k);
    sscanf(argv[2], "%Le", &ldR);

	gettimeofday(&tv, NULL);
	lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    getGraph(k, ldR, argv[3], argv[4]);

	gettimeofday(&tv, NULL);
	lTotalTime = tv.tv_sec * 1000 + tv.tv_usec / 1000 - lStartTime;
	printf("third step Time=%ld ms\n==============\n", lTotalTime);

    return 0;
}
