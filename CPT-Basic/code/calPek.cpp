/***************
calculate pek
****************/
#include <sys/time.h>
#include <math.h>
#include "common.h"
#include "myG.h"
#include "deTruss.h"
#include "calPek.h"

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
	long double *temp;
	long double *pw;
	long double *B;
	TPST_MAP_BY_XY *pstXZ = NULL;
	TPST_MAP_BY_XY *pstYZ = NULL;
	int i = 0;

	temp = new long double [iKe + 1];
	pw = new long double [iKe + 1];
	B = new long double [iKe + 1];

	pw[0] = -1;
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
		pw[i+1] = pstXZ->p * pstYZ->p;
		i++;
	}

	temp[0] = 1;
	for (i = 1; i <= iKe; i++)
	{
		temp[i] = temp[i - 1] * (1 - pw[i]);
	}

	vfDtE.clear();
	vfDtE.push_back(temp[iKe]);
	for (int l=1; l <= iKe; l++)
	{
		B[0] = 0;
		for (int i=1; i <= iKe; i++)
		{
			B[i] = temp[i-1]*pw[i] +B[i-1]*(1-pw[i]);
		}
		for (int i=0; i <= iKe; i++)
		{
			temp[i] = B[i];
		}
		vfDtE.push_back(temp[iKe]);
	}

	long double fSum = 0;
	for (int i=0; i <= iKe; i++)
	{
		fSum+= vfDtE[i];
	}

	if (fabs(fSum-1)>=1e-6)
	{
		printf("%d %d %.6Lf\n", x, y, fSum);
		debug_assert(0);
	}

	delete [] temp;
	delete [] pw;
	delete [] B;
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
int cal_OneEdgePek(int x, int y, myG &OmapG, vector <long double> &vfDtE)
{
    list<int> cn;
    int iNebCnt = 0;

    iNebCnt = OmapG.findNeb(x, y, cn);
    calOneEdgePekByNeib(x, y, cn, OmapG, vfDtE, iNebCnt);

    return iNebCnt;
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
int calPek_update(int x, int y, int z, int ke, vector <long double> &vldPekXY, myG &mapG)
{
    TPST_MAP_BY_XY *pstXZ = NULL;
    TPST_MAP_BY_XY *pstYZ = NULL;
	long double p_del = 0;
	long double *pet = NULL;

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
	pet = new long double[ke+2];
    if (ke >= vldPekXY.size())
    {
        printf("assert ke %d, size: %d x: %d, y: %d\n",
               ke, vldPekXY.size(), x, y);
        debug_assert(0);
    }
	if (p_del<0.5)
	{
		vldPekXY[0] = vldPekXY[0]/(1-p_del);

		for (int i=1; i <= ke; i++)
		{
				vldPekXY[i] = (vldPekXY[i]/(1.0-p_del)-p_del*vldPekXY[i-1]/(1.0-p_del));
		}
	}
	else
    {
        /* new must be 0 */
		pet[ke + 1] = 0;
		for (int i = ke + 1; i>=1; i--)
		{
			pet[i-1] = (vldPekXY[i]-pet[i]*(1.0-p_del))/p_del;
		}
		for (int i=0; i<=ke; i++)
		{
			vldPekXY[i] = pet[i];
		}
	}
    /* clear old max value */
    vldPekXY[ke + 1] = 0;
    //vldPekXY.resize(ke + 1);

	long double  sum = 0;
	for (int i=0; i<=ke; i++)
	{
		sum += vldPekXY[i];
	}
	if (fabs(sum-1) > 1e-8)
    {
        list<int> cn;
        list<int>::iterator itNeb;
        int iNeibCnt = 0;

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
	delete [] pet;
	return 0;
}
