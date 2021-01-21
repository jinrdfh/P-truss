#pragma once
/***************
truss decomposition
****************/


/* truss decomposition */

int deTruss(myG &mapInitG, MP_DECOMP_G &mpDeG);

/*****************
input:
        mapType mpA
        mapType mpB
output:
        mapType mpA
description:
        map a = a + b
******************/
template <typename mapType>
bool deTruss_mapAdd(mapType &mpA, mapType &mpB)
{
    typename mapType::iterator itmapT;

    for (itmapT = mpB.begin(); itmapT != mpB.end(); itmapT++)
    {
        debug_assert(mpA.find(itmapT->first) == mpA.end());
        mpA[itmapT->first] = itmapT->second;
    }
    return 0;
}
/*****************
input:
        mapType mpA
        mapType mpB
output:
        mapType mpA
description:
        map a = b
******************/
template <typename mapType>
bool deTruss_mapMir(mapType &mpA, mapType &mpB)
{
    typename mapType::iterator itmapT;
    mpA = mpB;
    for (itmapT = mpB.begin(); itmapT != mpB.end(); itmapT++)
    {
        //debug_assert(mpA.find(itmapT->first) == mpA.end());

        mpA[pair<int, int>(itmapT->first.second, itmapT->first.first)] = itmapT->second;
        //mpA[itmapT->first] = itmapT->second;
    }
    return 0;
}
/*****************
input:
        mapType mpA
output:
        mapType mpA
description:
        map a = b
******************/
template <typename mapType>
bool deTruss_mapCheckMir(mapType &mpA)
{
    typename mapType::iterator itmapT;
    for (itmapT = mpA.begin(); itmapT != mpA.end(); itmapT++)
    {
        //debug_assert(mpA.find(itmapT->first) == mpA.end());

        debug_assert(mpA.find(pair<int, int>(itmapT->first.second, itmapT->first.first)) != mpA.end());
        //mpA[itmapT->first] = itmapT->second;
    }
    return 0;
}
/*****************
input:
        mapType mpA
        mapType mpB
output:
        mapType mpA
description:
        map a = a - b
******************/
template <typename mapType>
bool deTruss_mapSub(mapType &mpA, mapType &mpB)
{
    typename mapType::iterator itmapT;

    for (itmapT = mpB.begin(); itmapT != mpB.end(); itmapT++)
    {
        if (mpA.find(itmapT->first) == mpA.end())
        {
            printf("error (%d, %d)\n", itmapT->first.first, itmapT->first.second);
            debug_assert(0);
        }
        mpA.erase(itmapT->first);
    }
    return 0;
}
/*****************
input:
        mapType mpA
        mapType mpB
output:
        mapType mpA
description:
        map a = a - b
******************/
template <typename mapType>
bool deTruss_mapSub(mapType &mpA, int x, int y)
{
    mpA.erase(pair<int, int>(x, y));
    mpA.erase(pair<int, int>(y, x));
    return true;
}
/*****************
input:
        mapType mpA
        mapType mpB
output:
        mapType mpA
description:
        map a = b - c, save key
******************/
template <typename mapType>
bool deTruss_mapSub(mapType &mpA, mapType &mpB, mapType &mpC)
{
    typename mapType::iterator itmapT;

    for (itmapT = mpB.begin(); itmapT != mpB.end(); itmapT++)
    {
        if (mpC.find(itmapT->first) == mpC.end())
        {
            mpA[itmapT->first];
        }
    }
    return 0;
}
/*****************
input:
        mapType mpA
        mapType mpB
output:
        mapType mpA
description:
        check mpA > mpB
******************/
template <typename mapType>
bool deTruss_checkMapSub(mapType &mpA, mapType &mpB)
{
    mapType mpC;

    mpC = mpA;
    deTruss_mapSub(mpC, mpB);
    return 0;
}

/*****************
input:
        mapType mpA
        mapType mpB
output:
        mapType mpA
description:
        map a = a intersect b
******************/
template <typename mapType>
bool deTruss_mapInt(mapType &mpA, mapType &mpB)
{
    typename mapType::iterator itmapT;
    mapType mpC;

    for (itmapT = mpA.begin(); itmapT != mpA.end(); itmapT++)
    {
        if (mpB.find(itmapT->first) != mpB.end())
        {
            mpC[itmapT->first] = mpB[itmapT->first];
        }
    }
    mpA.swap(mpC);
    mpC.clear();
    return 0;
}






