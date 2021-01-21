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
#include "file.h"
#include "deTruss.h"
#include "calPek.h"
#include "delE.h"

/* double no direct */
LIST_DECOMP_G g_lstDecompG;
long double g_ldAccu;
char *g_pcDataLabel;

/*****************
input:
        char *szFileName data file name
        long double ldMinP
description:
        global init
******************/
int init(char *szFileName, long double ldMinP, myG &oMainG)
{
    g_lstDecompG.setOrderOffset(getStructOffset(TPST_DECOMP_G, k), NULL);

    // printf("before object init\n");
    oMainG.simpleInit();
    // printf("after object init\n");
    file_fillG(szFileName, ldMinP, oMainG);
    // printf("start init\n");
    oMainG.otherInit();
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
    LIST_DECOMP_G::iterator itDeG;
	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lDetrussTime = 0;
	long lDelGTime = 0;
	long lSaveTime = 0;
	long lTotalTime = 0;
	long double ldMinP = 0;
	int iKSpace = 0;
	myG oMainG;

	gettimeofday(&tv, NULL);
	lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    // printf("start read file\n");
    sscanf(argv[3], "%Lf", &ldMinP);
    sscanf(argv[4], "%Lf", &g_ldAccu);
    sscanf(argv[5], "%d", &iKSpace);
    g_pcDataLabel = argv[6];
    // printf("get minimum p: %Lf accuracy: %Lf k space: %d\n",
    //        ldMinP, g_ldAccu, iKSpace);
    debug_assert(( 0 <= ldMinP ) && (1 > ldMinP));
    debug_assert(( 0 <= g_ldAccu ) && (1 > g_ldAccu));
    init(argv[1], ldMinP, oMainG);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lTotalTime = lCurTime - lStartTime;
	// printf("read file Time=%ld ms\n", lTotalTime);

    //printf("map size: %d eidMax: %d\n", oMainG.m_mpG.size(), oMainG.m_iMaxEId);
    // printf("start deTruss\n");
	lStartTime = lCurTime;
	/** first deTruss **/
    deTruss_ByP(oMainG, g_lstDecompG, ldMinP);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lDetrussTime = lCurTime - lStartTime;
	// printf("deTruss Time=%ld ms\n", lDetrussTime);

	lStartTime = lCurTime;
	/** save deTruss result **/
    file_saveDeTruss(g_lstDecompG, argv[2]);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lTotalTime = lCurTime - lStartTime;
	// printf("save deTruss Time=%ld ms\n", lTotalTime);

    // printf("start del\n");
	lStartTime = lCurTime;
	/** delete G **/
    lSaveTime = delE_partG(g_lstDecompG, argv[2], iKSpace, oMainG);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lDelGTime = lCurTime - lStartTime;
	// printf("Delete G Time=%ld ms\n", lDelGTime - lSaveTime);

	printf("building APX index Time=%ld ms\n", lDetrussTime + lDelGTime - lSaveTime);
    return 0;
}
