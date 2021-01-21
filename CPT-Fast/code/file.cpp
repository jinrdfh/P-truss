/***************
input file output file function
****************/

#include "common.h"
#include "myG.h"
#include "deTruss.h"
#include "delE.h"
#include "file.h"

extern long double g_ldAccu;
extern char *g_pcDataLabel;

/* replace 0 */
int g_iOffSetPid = 1;

/*****************
input:
        char *szFileName
        long double ldMinP
        myG &mpG
description:
        read file and fill the list g_initG
******************/
int file_fillG(char *szFileName, long double ldMinP, myG &mpG)
{
    FILE *fp = NULL;
    int iTempX = 0;
    int iTempY = 0;
    char szBuffer[ONE_LINE_BUFFER] = {0};
    long double fP = 0;

    if (NULL == szFileName)
    {
        fp = fopen(DATA_PATH, "rt");
    }
    else
    {
        fp = fopen(szFileName, "rt");
    }
    if (NULL == fp)
    {
        printf("error no file: %s or %s\n", DATA_PATH, szFileName);
        debug_assert(0);
    }
    while (fgets(szBuffer, ONE_LINE_BUFFER - 1, fp) > 0)
    {
        sscanf(szBuffer, "%d,%d,%Lf", &iTempX, &iTempY, &(fP));
        if ((fP < 0) || (fP > 1))
        {
            // printf("error %d,%d,%Lf\n", iTempX, iTempY, fP);
            continue;
        }
        else if (fP < ldMinP)
        {
            continue;
        }

        if (iTempX == iTempY)
        {
            continue;
        }

        mpG.add(iTempX + g_iOffSetPid, iTempY + g_iOffSetPid, fP);
    }
    fclose(fp);
    return 0;
}
/*****************
input:
        int k
        LT_PB_G &ltPbG
        char *szSaveFile
description:
        save pbG
******************/
int file_savePbG(int k, LT_PB_G &ltPbG, char *szSaveFile, myG &mpG)
{
    char szFileName[FILE_NAME_BUFFER] = {0};
    FILE *fp = NULL;
    LT_PB_G::iterator itPbG;
    list<int>::iterator itE;
    bool bOpenFlag = false;
    pair<int, int> paXY;

    for (itPbG = ltPbG.begin(); itPbG != ltPbG.end(); itPbG++)
    {
        if (!bOpenFlag)
        {
            bOpenFlag = true;
            /* open */
            if (NULL == szSaveFile)
            {
                sprintf(szFileName, "../result/%s-%d-truss.index", g_pcDataLabel, k);
            }
            else
            {
                sprintf(szFileName, "%s/%s-%d-truss.index", szSaveFile, g_pcDataLabel, k);
            }
            fp = fopen(szFileName, "wt+");
            debug_assert(NULL != fp);
        }
        fprintf(fp, "r,%e\n", itPbG->fKey);
        for (itE = itPbG->ltESet.begin(); itE != itPbG->ltESet.end(); itE++)
        {
            paXY = mpG.m_szG[*itE].paXY;
            fprintf(fp, "%d,%d\n", paXY.first - g_iOffSetPid, paXY.second - g_iOffSetPid);
        }
    }
    if (bOpenFlag)
    {
        fclose(fp);
        fp = NULL;
    }
    return 0;
}

/*****************
input:
        MP_DECOMP_G &mpDeG
output:
        none
description:
        save truss decomposition
******************/
int file_saveDeTruss(MP_DECOMP_G &mpDeG, char *szTrussPath, myG &mpG)
{
    MP_DECOMP_G::iterator itDeG;
    list<int>::iterator itE;
    pair<int, int> paXY;
    FILE *fp = NULL;
    int x = 0;
    int y = 0;
    char szFileName[FILE_NAME_BUFFER] = {0};

    sprintf(szFileName, "%s/%s-%s", szTrussPath, g_pcDataLabel, TRUSS_FILE);
    fp = fopen(szFileName, "wt");

    if (NULL == fp)
    {
        printf("error write file: %s\n", szFileName);
        debug_assert(0);
    }
    // printf("debug k\n");
    for (itDeG = mpDeG.begin(); itDeG != mpDeG.end(); itDeG++)
    {
        fprintf(fp, "k,%d\n", itDeG->first);
        printf("\"%d\" ", itDeG->first);
        for (itE = itDeG->second.begin(); itE != itDeG->second.end(); itE++)
        {
            paXY = mpG.m_szG[*itE].paXY;

            fprintf(fp, "%d,%d\n", paXY.first - g_iOffSetPid, paXY.second - g_iOffSetPid);
        }
    }
    // printf("end debug k\n");
    fclose(fp);
    fp = NULL;
    return 0;
}
