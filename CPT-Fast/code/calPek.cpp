/***************
calculate pek
****************/
#include <sys/time.h>
#include <math.h>
#include "common.h"
#include "myG.h"
#include "deTruss.h"
#include "calPek.h"

extern long g_lRefreshCnt;

extern long g_lBuildNewTime;
extern long g_lBuildNewCnt;
extern long g_lBuildOldTime;
extern long g_lBuildOldCnt;
extern long g_lBuildOtherTime;

/*****************
input:
        int x
        int y
        list<int> &cn
        myG &mapG
        vector <long double> &vfDtE
        int iKe
output:
        vector <long double> &vfDtE
description:
        calculate pek
******************/
int calOneEdgePekByNeib(int x, int y, list<int> &cn, myG &mapG, vector <long double> &vfDtE, int iKe)
{
    list<int>::iterator itLst;
	long double *szTemp;
	long double *szPw;
	long double ldSum = 0;
	// long double *B;
	TPST_MAP_BY_XY *pstXZ = NULL;
	TPST_MAP_BY_XY *pstYZ = NULL;
	int i = 0;
	int j = 0;

	// szTemp = new long double [iKe + 1];
	//szPw = new long double [iKe + 1];
	//B = new long double [iKe + 1];

    szTemp = (long double *)malloc((iKe + 1) * sizeof(long double));
    debug_assert(NULL != szTemp);
    memset(szTemp, 0, (iKe + 1) * sizeof(long double));

    szPw = (long double *)malloc((iKe + 1) * sizeof(long double));
    debug_assert(NULL != szPw);
    memset(szPw, 0, (iKe + 1) * sizeof(long double));

	// szPw[0] = -1;
	i = 0;
	for (itLst = cn.begin(); itLst != cn.end(); itLst++)
	{
	    pstXZ = mapG.findNode(x, *itLst);
	    pstYZ = mapG.findNode(y, *itLst);
		debug_assert(NULL != pstXZ);
		if (NULL == pstYZ)
        {
            printf("error (%d, %d)\n", y, *itLst);
            debug_assert(0);
        }
		szPw[i+1] = pstXZ->p * pstYZ->p;
		i++;
	}

	szTemp[0] = 1;
	//debug_assert(0 == szTemp[iKe]);
	for (i = 1; i <= iKe; i++)
	{
		for (j = i; j > 0; j--)
        {
            szTemp[j] = szTemp[j-1] * szPw[i] + szTemp[j] * (1.0 - szPw[i]);
        }
        szTemp[0] = szTemp[0] * (1.0 - szPw[i]);
	}

	vfDtE.clear();
	vfDtE.resize(iKe + 1);
	for (i = 0; i <= iKe; ++i)
	{
	    vfDtE[i] = szTemp[i];
	}

	for (i = 0; i <= iKe; ++i)
	{
	    ldSum += szTemp[i];
	}
	if (fabs(ldSum-1)>=1e-6)
	{
		printf("%d %d iKe: %d, %.6Lf\n", x, y, iKe, ldSum);
		debug_assert(0);
	}

	free(szTemp);
	free(szPw);
	// delete [] B;
    return 0;
}
/*****************
input:
        int x
        int y
        int z
        int ke
        vector <long double> &vldPekXY
        myG &mapG
description:
        update one edge
        x, y is update edge
        x, y, z is vanished triangle
        ke is support number after vanished
******************/
int calPek_addOne(int x, int y, int z, int iKe, vector <long double> &vldPekXY, myG &mapG, int *piVDtLen)
{
    TPST_MAP_BY_XY *pstXZ = NULL;
    TPST_MAP_BY_XY *pstYZ = NULL;
	long double p_add = 0;

	int i = 0;

	pstXZ = mapG.findNode(x, z);
	pstYZ = mapG.findNode(y, z);
    if (pstXZ == NULL)
    {
        printf("assert x: %d, z: %d\n", x, z);
        debug_assert(0);
    }
    if (pstYZ == NULL)
    {
        printf("assert y: %d, z: %d\n", y, z);
        debug_assert(0);
    }

	p_add = pstXZ->p * pstYZ->p;

    if (iKe < *piVDtLen)
    {
        printf("assert ke %d, size: %d x: %d, y: %d\n",
               iKe, vldPekXY.size(), x, y);
        debug_assert(0);
    }

    vldPekXY.push_back(vldPekXY[iKe - 1] * p_add);

    for (i = iKe - 1; i > 0; --i)
    {
        vldPekXY[i] = vldPekXY[i - 1] * p_add + vldPekXY[i] * (1.0 - p_add);
    }
    vldPekXY[0] = vldPekXY[0] * (1.0 - p_add);

    *piVDtLen = *piVDtLen + 1;
    debug_assert(vldPekXY.size() == *piVDtLen);

	long double  sum = 0;
	for (i = 0; i<=iKe; i++)
	{
		sum += vldPekXY[i];
	}
	if (fabs(sum - 1) > 1e-8)
    {
        list<int> cn;
        list<int>::iterator itNeb;
        int iNeibCnt = 0;

        debug_assert(0);

        iNeibCnt = mapG.findNeb(x, y, cn);
        /* delete z */
        for (itNeb = cn.begin(); itNeb != cn.end(); itNeb++)
        {
            if (*itNeb == z)
            {
                cn.erase(itNeb);
                break;
            }
        }
        calOneEdgePekByNeib(x, y, cn, mapG, vldPekXY, iNeibCnt - 1);

    }
	return 0;
}

/*****************
input:
        list<int> &liSupG
        myG &OmapG
output:
        vector <long double> &vfDtE
description:
        calculate pek
******************/
int calPek_partL(list<int> &liSupG, myG &OmapG, myG &oUpG, MAP_BASIC_G &mapCalG)
{
    int x = 0;
    int y = 0;
    list<int>::iterator itG;
    list<int>::iterator itP;
    pair<int, int> paXY;
    TPST_MAP_BY_XY *pstNode = NULL;
    TPST_MAP_BY_XY *pstUpNode = NULL;
    int iDebugI = 0;
    list<int> lstNeib;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;

    //printf("DEBUG begin cal part\n");

    for (itG = liSupG.begin(); itG != liSupG.end(); itG++)
    {
        /* init */
        pstNode = OmapG.findNode(*itG);
        if (NULL == pstNode)
        {
            if (OmapG.m_mpBasicG.find(pair <int, int> (x, y)) == OmapG.m_mpBasicG.end())
            {
                printf("not in it x: %d y: %d\n", x, y);
            }
            else
            {
                printf("in it\n");
            }
            debug_assert(0);
        }

        paXY = pstNode->paXY;
        x = paXY.first;
        y = paXY.second;
        pstNode->vldPek.clear();

        if (NULL != oUpG.m_szG)
        {
            //printf("DEBUG not k_max\n");
            /* not k_max */
            pstUpNode = oUpG.findNode(*itG);
            debug_assert(NULL != pstUpNode);


            if (0 < pstUpNode->iVTgLen)
            {
                gettimeofday(&tv, NULL);
                lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                lstNeib.clear();
                OmapG.findNeb(x, y, lstNeib, mapCalG);

                pstNode->vldPek.swap(pstUpNode->vldTriggerPek);
                pstNode->iVPLen = pstUpNode->iVTgLen;
                pstNode->iSup = pstUpNode->iVTgLen - 1;
                gettimeofday(&tv, NULL);
                lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                g_lBuildOtherTime += lCurTime - lStartTime;

                if (!lstNeib.empty())
                {
                    for (itP = lstNeib.begin(); itP != lstNeib.end(); ++itP)
                    {
                        gettimeofday(&tv, NULL);
                        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                        calPek_addOne(x, y, *itP, pstNode->iVPLen, pstNode->vldPek, OmapG, &(pstNode->iVPLen));
                        gettimeofday(&tv, NULL);
                        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                        g_lBuildOldTime += lCurTime - lStartTime;
                        pstNode->iSup = pstNode->iVPLen - 1;
                        ++g_lBuildOldCnt;
                    }
                }

                continue;
            }
        }

        /* normal build */
        gettimeofday(&tv, NULL);
        lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        cal_OneEdgePek(x, y, OmapG, pstNode->vldPek,  &(pstNode->iVPLen));
        gettimeofday(&tv, NULL);
        lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        g_lBuildNewTime += lCurTime - lStartTime;
        pstNode->iSup = pstNode->iVPLen - 1;

        ++g_lBuildNewCnt;

    }

    return 0;
}

/*****************
input:
        int x
        int y
        MAP_G mapG
output:
        vector <long double> vfDtE
description:
        show MAP_G mapG
******************/
int cal_OneEdgePek(int x, int y, myG &OmapG, vector <long double> &vfDtE, int *piVDtLen)
{
    list<int> cn;
    int iNebCnt = 0;

    iNebCnt = OmapG.findNeb(x, y, cn);
    calOneEdgePekByNeib(x, y, cn, OmapG, vfDtE, iNebCnt);

    *piVDtLen = iNebCnt + 1;
    return iNebCnt;
}

/*****************
input:
        int x
        int y
        int z
        int iKe
        vector <long double> &vldPekXY
        myG &mapG
description:
        update one edge
        x, y is update edge
        x, y, z is vanished triangle
        ke is support number after vanished
******************/
int calPek_update(int x, int y, int z, int iKe, vector <long double> &vldPekXY, myG &mapG, int *piVDtLen)
{
    TPST_MAP_BY_XY *pstXZ = NULL;
    TPST_MAP_BY_XY *pstYZ = NULL;
	long double p_del = 0;
	long double *pet = NULL;
	//long double *szOV = NULL;
	int i = 0;
	int iDebugFlag = 0;

	pstXZ = mapG.findNode(x, z);
	pstYZ = mapG.findNode(y, z);
    if (pstXZ == NULL)
    {
        printf("assert x: %d, z: %d\n", x, z);
        debug_assert(0);
    }
    if (pstYZ == NULL)
    {
        printf("assert y: %d, z: %d\n", y, z);
        debug_assert(0);
    }

	p_del = pstXZ->p * pstYZ->p;

	/* pet[ke + 1] be 0 */
	// pet = new long double[iKe + 2];

    pet = (long double *)malloc((iKe + 2) * sizeof(long double));
    debug_assert(NULL != pet);
    memset(pet, 0, (iKe + 2) * sizeof(long double));

    /*szOV = (long double *)malloc((iKe + 2) * sizeof(long double));
    debug_assert(NULL != szOV);
    memset(szOV, 0, (iKe + 1) * sizeof(long double));*/


    if (iKe >= *piVDtLen)
    {
        printf("assert ke %d, size: %d x: %d, y: %d\n",
               iKe, vldPekXY.size(), x, y);
        debug_assert(0);
    }

    /*szOV[iKe + 1] = vldPekXY[iKe + 1];
    for (i = iKe; i > -1; --i)
    {
        szOV[i] = vldPekXY[i] - vldPekXY[i + 1];
    }*/
	if (p_del<0.5)
	{
	    iDebugFlag = 0;
		vldPekXY[0] = vldPekXY[0]/(1-p_del);

		for (i=1; i <= iKe; i++)
		{
            vldPekXY[i] = (vldPekXY[i]-p_del*vldPekXY[i-1])/(1.0-p_del);
		}
	}
	else
    {
        iDebugFlag = 1;
        /* new must be 0 */
		pet[iKe + 1] = 0;
		for (i = iKe + 1; i > 0; --i)
		{
			pet[i-1] = (vldPekXY[i]-pet[i]*(1.0-p_del))/p_del;
		}
        for (i = iKe; i > -1; --i)
        {
            vldPekXY[i] = pet[i];
        }
	}
    /* clear old max value */
    vldPekXY[iKe + 1] = 0;
    *piVDtLen = *piVDtLen - 1;
    vldPekXY.pop_back();
    debug_assert(vldPekXY.size() == *piVDtLen);

    //vldPekXY.resize(ke + 1);

	long double  ldSum = 0;
    for (i = iKe; i > -1; --i)
    {
        ldSum += vldPekXY[i];
    }
	if (fabs(ldSum - 1) > 1e-8)
    {
        list<int> cn;
        list<int>::iterator itNeb;
        int iNeibCnt = 0;

        /*printf("DEBUG sum: %Le flag: %d\n", ldSum, iDebugFlag);
        debug_assert(0);*/
        g_lRefreshCnt++;

        iNeibCnt = mapG.findNeb(x, y, cn);
        /* delete z */
        for (itNeb = cn.begin(); itNeb != cn.end(); itNeb++)
        {
            if (*itNeb == z)
            {
                cn.erase(itNeb);
                break;
            }
        }
        calOneEdgePekByNeib(x, y, cn, mapG, vldPekXY, iNeibCnt - 1);

    }
	free(pet);
	//free(szOV);
	return 0;
}

