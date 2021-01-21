/***************
truss decomposition of probabilistic graphs
first step
generate several k and r
main cpp
****************/
#include <math.h>
#include <sys/time.h>
#include "common.h"
#include "myG.h"
#include "deTruss.h"
#include "delE.h"
#include "file.h"
#include "calPek.h"

/* double no direct */
MP_DECOMP_G g_lstDecompG;
char *g_pcDataLabel;
long g_lRefreshCnt;

long g_lBuildNewTime;
long g_lBuildNewCnt;
long g_lBuildOldTime;
long g_lBuildOldCnt;
long g_lBuildOtherTime;

/*****************
input:
        char *szFileName data file name
        long double ldMinP
description:
        global init
******************/
int init(char *szFileName, long double ldMinP, myG &oMainG)
{
    //g_lstDecompG.setOrderOffset(getStructOffset(TPST_DECOMP_G, k), NULL);

    // printf("before object init\n");
    oMainG.beforeDataInit();
    // printf("after object init\n");
    file_fillG(szFileName, ldMinP, oMainG);
    // printf("start init\n");
    oMainG.afterDataInit();
    return 0;
}

/*****************
input:
        Graph file name
        output data file name
        long double minimum possibility
        long double accuracy
        int kSpace
        dataset label
description:
        main function
******************/
int main(int argc, char *argv[])
{
    //LIST_DECOMP_G::iterator itDeG;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lDetrussTime = 0;
	long lDelGTime = 0;
	long lSaveTime = 0;
	long lTotalTime = 0;

	myG oMainG;

	debug_assert(4 == argc);

	gettimeofday(&tv, NULL);
	lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    // printf("start read file\n");
    g_pcDataLabel = argv[3];
    init(argv[1], 0, oMainG);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lTotalTime = lCurTime - lStartTime;
	// printf("read file Time=%ld ms\n", lTotalTime);

    /*printf("DEBUG INIT (%d, %d) eid: %d\n",
           oMainG.m_mpBasicG.begin()->first.first,
           oMainG.m_mpBasicG.begin()->first.second,
           oMainG.m_mpBasicG.begin()->second.first);*/

    // printf("start deTruss\n");
	lStartTime = lCurTime;
	/** first deTruss **/
    deTruss(oMainG, g_lstDecompG);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lDetrussTime = lCurTime - lStartTime;
	// printf("deTruss Time=%ld ms\n", lDetrussTime);

    /*printf("DEBUG DETRUSS (%d, %d) eid: %d\n",
           oMainG.m_mpBasicG.begin()->first.first,
           oMainG.m_mpBasicG.begin()->first.second,
           oMainG.m_mpBasicG.begin()->second.first);*/

    // printf("start del\n");
	lStartTime = lCurTime;
	/** delete G **/
    lSaveTime = delE_partG(g_lstDecompG, argv[2], oMainG);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lDelGTime = lCurTime - lStartTime;
	// printf("Delete G Time=%ld ms\n", lDelGTime - lSaveTime);

	// printf("DEBUG refresh cnt: %ld ratio: %.3f\n", g_lRefreshCnt, g_lRefreshCnt / (oMainG.m_iMaxEId - 1.0));
	// printf("DEBUG build old cnt: %ld time: %ld ms\n new cnt: %ld time: %ld ms other time: %ld ms\n",
 //        g_lBuildOldCnt, g_lBuildOldTime, g_lBuildNewCnt, g_lBuildNewTime, g_lBuildOtherTime);

	printf("CPT-Fast Time=%ld ms\n", lDetrussTime + lDelGTime - lSaveTime);
    return 0;
}
