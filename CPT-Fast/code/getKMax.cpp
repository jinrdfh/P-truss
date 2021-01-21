/***************
get kmax
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
MP_DECOMP_G g_lstDecompG;
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
    //g_lstDecompG.setOrderOffset(getStructOffset(TPST_DECOMP_G, k), NULL);

    printf("before object init\n");
    oMainG.simpleInit();
    printf("after object init\n");
    file_fillG(szFileName, ldMinP, oMainG);
    printf("start init\n");
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
    //LIST_DECOMP_G::iterator itDeG;

	struct timeval tv;
	long lStartTime = 0;
	long lCurTime = 0;
	long lDetrussTime = 0;
	long lDelGTime = 0;
	long lSaveTime = 0;
	long lTotalTime = 0;

	myG oMainG;

	//debug_assert(4 == argc);

	gettimeofday(&tv, NULL);
	lStartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    printf("start read file\n");
    //g_pcDataLabel = argv[3];
    init(argv[1], 0, oMainG);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lTotalTime = lCurTime - lStartTime;
	printf("read file Time=%ld ms\n", lTotalTime);

    printf("start deTruss\n");
	lStartTime = lCurTime;
	/** first deTruss **/
    deTruss(oMainG, g_lstDecompG);
	gettimeofday(&tv, NULL);
	lCurTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	lDetrussTime = lCurTime - lStartTime;
	printf("deTruss Time=%ld ms\n", lDetrussTime);

    return 0;
}
