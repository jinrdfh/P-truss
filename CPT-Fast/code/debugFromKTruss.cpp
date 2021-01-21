/***************
truss decomposition of probabilistic graphs
debug from k-truss
main cpp
****************/
#include "common.h"
#include "deTruss.h"
#include "file.h"
#include "delE.h"

/* double no direct */
MAP_G g_initG;
map<int, list<int> > g_maplstPNe;
pair <int, int> *ppaFindEdgeByEid;
/*****************
input:
        char *szFileName data file name
description:
        global init
******************/
int init(char *szFileName)
{
    file_read(szFileName);
    printf("start init\n");
    file_GlobalInit(g_initG);
    return 0;
}

/*****************
input:
        Graph file name
        k
        k-truss file name
description:
        main function
******************/
int main(int argc, char *argv[])
{
    int k = 0;
    int x = 680;
    int y = 2384;
    MAP_PB_G mapPbG;
    MAP_PB_G mapUpPbG;
    MAP_G mapG;
    list<int> cnInit;
    list<int> cnK;

    init(argv[1]);
    mapG = g_initG;
    sscanf(argv[2], "%d", &k);
    printf("get k: %d\n", k);
    deTrussByK(g_maplstPNe, mapG, k);
    cnInit = deTruss_FindComNeib(x, y, g_initG, g_maplstPNe);
    cnK = deTruss_FindComNeib(x, y, mapG, g_maplstPNe);
    printf("init neb: %d k neb: %d\n", cnInit.size(), cnK.size());
    printf("start get PbG\n");
    file_getPbG(mapUpPbG, argv[3]);
    printf("start delete\n");
    delE_oneG(k - 2, mapG, mapPbG, &mapUpPbG);
    return 0;
}
