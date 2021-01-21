/***************
show something
****************/

#include "common.h"
#include "show.h"
#include "deTruss.h"

extern MAP_G g_initG;
extern map<int, list<int> > g_maplstPNe;
extern LIST_DECOMP_G g_lstDecompG;

/*****************
input:
        HAST_EDGE_BY_SUP lstFindEdgeBySup
description:
        show list lstFindEdgeBySup
******************/
int show_listEdgeBySup(HAST_EDGE_BY_SUP &lstFindEdgeBySup)
{
    HAST_EDGE_BY_SUP::iterator i;

    for (i = lstFindEdgeBySup.begin(); i != lstFindEdgeBySup.end(); i++)
    {
        printf("sup: %d, eid: %d\n", i->iSup, i->eid);
    }
    return 0;
}
/*****************
input:
        MAP_G mapG
description:
        show MAP_G mapG
******************/
int show_mapG(MAP_G &mapG)
{
    MAP_G::iterator i;

    for (i = mapG.begin(); i != mapG.end(); i++)
    {
        printf("eid: %d, %d, %d, f: %Lf\n",
               i->second.eid, i->first.first, i->first.second, i->second.p);
    }
    return 0;
}
/*****************
input:
        none
description:
        show list g_maplstPNe
******************/
int show_listNeib()
{
    map<int, list<int> >::iterator i;
    list<int>::iterator j;

    for (i = g_maplstPNe.begin(); i != g_maplstPNe.end(); i++)
    {
        printf("i:%d ", i->first);
        for (j = i->second.begin(); j != i->second.end(); j++)
        {
            printf("%d ", *j);
        }
        printf("\n");
    }
    return 0;
}
/*****************
input:
        none
description:
        show neighbor probability
******************/
int show_NeibP(int x, int y, MAP_G &mapG, map<int, list<int> > &maplstPNe)
{
    list<int> cn;
    list<int>::iterator itLst;
	TPST_MAP_BY_XY stXZ, stYZ;

    cn = deTruss_FindComNeib(x, y, mapG, maplstPNe);
	for (itLst = cn.begin(); itLst != cn.end(); itLst++)
	{
		stXZ = mapG[pair<int, int>(x, *itLst)];
		stYZ = mapG[pair<int, int>(y, *itLst)];

		printf("(%d, %d): %Lf\n", x, *itLst, stXZ.p);
		printf("(%d, %d): %Lf\n", y, *itLst, stYZ.p);
	}
    return 0;
}
/*****************
input:
        none
description:
        show list lstDecompG
******************/
int show_mapDeG(LIST_DECOMP_G lstDecompG)
{
    LIST_DECOMP_G::iterator i;
    MAP_G::iterator j;

    for (i = lstDecompG.begin(); i != lstDecompG.end(); i++)
    {
        printf("k: %d\n", i->k);
        show_mapG(i->mapSubG);
    }
    return 0;
}
