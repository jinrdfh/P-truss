#pragma once
/***************
calculate pek
****************/

int calPek_update(int x, int y, int z, int ke, vector <long double> &vldPekXY, myG &mapG);
int cal_OneEdgePek(int x, int y, myG &mapG, vector <long double> &vfDtE);

/*****************
input:
        MAP_BASIC_G &mapSupG
        myG &OmapG
description:
        calculate myG OmapG part pek, sup, bReadyDel
******************/
template <typename tpMp>
int calPek_part(tpMp &mapSupG, myG &OmapG)
{
    int x = 0;
    int y = 0;
    typename tpMp::iterator itG;
    TPST_MAP_BY_XY *pstNode = NULL;

    for (itG = mapSupG.begin(); itG != mapSupG.end(); itG++)
    {
        x = itG->first.first;
        y = itG->first.second;
        if (x < y)
        {
            /* init */
            pstNode = OmapG.findNode(x, y);
            if (NULL == pstNode)
            {
                if (OmapG.m_mpBasicG.find(itG->first) == OmapG.m_mpBasicG.end())
                {
                    printf("not in it x: %d y: %d\n", x, y);
                }
                else
                {
                    printf("in it\n");
                }
                debug_assert(0);
            }

            pstNode->iSup = cal_OneEdgePek(x, y, OmapG, pstNode->vldPek);
        }
    }

    return 0;
}
