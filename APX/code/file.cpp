/***************
input file output file function
****************/

#include "common.h"
#include "myG.h"
#include "file.h"
#include "delE.h"

extern long double g_ldAccu;
extern char *g_pcDataLabel;

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

    /* replace 0 */
    int iSubPid = 4036538;

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
            printf("error %d,%d,%Lf\n", iTempX, iTempY, fP);
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

        if (0 == iTempX)
        {
            iTempX = iSubPid;
        }
        if (0 == iTempY)
        {
            iTempY = iSubPid;
        }

        mpG.add(iTempX, iTempY, fP);
    }
    fclose(fp);
    return 0;
}
/*****************
input:
        int k
        MAP_PB_G_BY_KEY &mapPbG
        char *szSaveFile
description:
        save pbG
******************/
int file_savePbG(int k, MAP_PB_G &mapPbG, char *szSaveFile)
{
    char szFileName[FILE_NAME_BUFFER] = {0};
    FILE *fp = NULL;
    MAP_PB_G::iterator itMapG;
    list<pair<int, int> >::iterator itLstPa;
    bool bOpenFlag = false;

    for (itMapG = mapPbG.begin(); itMapG != mapPbG.end(); itMapG++)
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
        fprintf(fp, "r,%Le\n", itMapG->first);
        for (itLstPa = itMapG->second.begin(); itLstPa != itMapG->second.end(); itLstPa++)
        {
            fprintf(fp, "%d,%d\n", itLstPa->first, itLstPa->second);
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
        LIST_DECOMP_G &lstDeG
output:
        none
description:
        save truss decomposition
******************/
int file_saveDeTruss(LIST_DECOMP_G &lstDeG, char *szTrussPath)
{
    LIST_DECOMP_G::iterator itDeG;
    MAP_BASIC_G::iterator itmpG;
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
    for (itDeG = lstDeG.begin(); itDeG != lstDeG.end(); itDeG++)
    {
        fprintf(fp, "k,%d\n", itDeG->k);
        // printf("\"%d\" ", itDeG->k);
        for (itmpG = itDeG->mapSubG.begin(); itmpG != itDeG->mapSubG.end(); itmpG++)
        {
            x = itmpG->first.first;
            y = itmpG->first.second;
            if (x < y)
            {
                fprintf(fp, "%d,%d\n", x, y);
            }
        }
    }
    // printf("end debug k\n");
    fclose(fp);
    fp = NULL;
    return 0;
}
