#pragma once
/***************
my list class
add function: insert by order
****************/
#include <stdio.h>
#include <list>
#include <map>

using namespace std;

#define getStructOffset(structName, memberName) ((long)&(((structName *)0)->memberName))

template<typename _Tp, typename _Mb, typename _MK>
class myList : public list <_Tp>
{
    typedef typename list <_Tp>::iterator itItem;
    typedef typename list <itItem>::iterator tpitPos;
    private:
        int iOrderOffset;
        map <_MK, list <itItem> > itemFind;

        static _MK defaltHanKey(_Mb tpOKey)
        {
            return tpOKey;
        }

        bool myErase(itItem itT)
        {
            _MK stKey;
            typename map <_MK, list <itItem> >::iterator itmapTemp;
            list <itItem> *pvTemp;
            typename list <itItem>::iterator itvIt;
            stKey = getOrderKey(&(*itT));
            itmapTemp = itemFind.find(stKey);

            /** abandon **/
            debug_assert(0);
            if (itmapTemp == itemFind.end())
            {
                return false;
            }
            pvTemp = &(itmapTemp->second);
            for (itvIt = pvTemp->begin(); itvIt != pvTemp->end(); itvIt++)
            {
                if (*itvIt == itT)
                {
                    break;
                }
            }
            if (itvIt == pvTemp->end())
            {
                return false;
            }
            pvTemp->erase(itvIt);
            if (pvTemp->empty())
            {
                /* delete key */
                itemFind.erase(stKey);
            }
            this->erase(itT);
            return true;
        }

        _MK getOrderKey(_Tp *pstDes)
        {
            _Mb stVal;
            _MK stRes;
            stVal = *((_Mb *)((void *)pstDes + iOrderOffset));
            stRes = m_funcHanKey(stVal);
            return stRes;
        }
    public:
        _MK (*m_funcHanKey)(_Mb tpOKey);
        bool setOrderOffset(int iOffset, _MK (*funcHanKey)(_Mb tpOKey))
        {
            this->iOrderOffset = iOffset;

            if (NULL == funcHanKey)
            {
                m_funcHanKey = defaltHanKey;
            }
            else
            {
                m_funcHanKey = funcHanKey;
            }
            return true;
        }
        /* ascending order */
        tpitPos insetByOrder(_Tp& __x)
        {
            _MK stKey;
            itItem lsitDes;
            list <itItem> lsTemp;
            list <itItem> *plsTemp;
            typename map < _MK, list <itItem> >::iterator itPos;
            pair< typename map < _MK, list <itItem> >::iterator, bool> ret;
            tpitPos itRetPos;

            stKey = getOrderKey(&__x);
            this->push_front(__x);
            lsitDes = this->begin();
            itPos = itemFind.find(stKey);
            if (itPos == itemFind.end())
            {
                /* new key */
                lsTemp.push_back(lsitDes);
                ret = itemFind.insert(pair<_MK, list <itItem> >(stKey, lsTemp));
                itPos = ret.first;
                itRetPos = itemFind[stKey].begin();
            }
            else
            {
                plsTemp = &(itPos->second);
                plsTemp->push_back(lsitDes);
                itRetPos = plsTemp->end();
                itRetPos--;
            }
            return itRetPos;
        }

        _Tp popInList()
        {
            list <itItem> *plsTemp;
            typename map < _MK, list <itItem> >::iterator itPos;
            _Tp stTempVal;
            itItem itT;

            itPos = itemFind.begin();
            if (itPos == itemFind.end())
            {
                debug_assert(0);
                return stTempVal;
            }
            plsTemp = &(itPos->second);
            itT = *(plsTemp->begin());
            stTempVal = *(itT);
            this->myDel(plsTemp->begin());

            return stTempVal;
        }
        _Tp getTop()
        {
            list <itItem> *plsTemp;
            typename map < _MK, list <itItem> >::iterator itPos;
            _Tp stTempVal = {0};
            itItem itT;

            itPos = itemFind.begin();
            if (itPos == itemFind.end())
            {
                return stTempVal;
            }
            plsTemp = &(itPos->second);
            itT = *(plsTemp->begin());

            return *(itT);
        }

        list <itItem> *getTopList()
        {
            list <itItem> *plsTemp;
            typename map < _MK, list <itItem> >::iterator itPos;

            itPos = itemFind.begin();
            if (itPos == itemFind.end())
            {
                debug_assert(0);
                return NULL;
            }
            plsTemp = &(itPos->second);

            return plsTemp;
        }

        bool myDel(tpitPos itPos)
        {
            _MK stKey;
            itItem itT;
            typename map <_MK, list <itItem> >::iterator itmapTemp;
            list <itItem> *plTemp;

            itT = *itPos;
            stKey = getOrderKey(&(*itT));
            itmapTemp = itemFind.find(stKey);
            if (itmapTemp == itemFind.end())
            {
                return false;
            }
            plTemp = &(itmapTemp->second);

            plTemp->erase(itPos);
            if (plTemp->empty())
            {
                /* delete key */
                itemFind.erase(stKey);
            }
            this->erase(itT);
            return true;
        }
        bool judgeChange(tpitPos itPos, _Tp& __x)
        {
            _MK stKey;
            _MK stNewKey;
            itItem itT;

            itT = *itPos;
            stKey = getOrderKey(&(*itT));
            stNewKey = getOrderKey(&__x);
            if (stKey == stNewKey)
            {
                /* no change */
                return false;
            }
            else
            {
                return true;
            }
        }

};


template<typename _Tp, typename _Mb, typename _MK, typename _Hs>
class myHsList : public myList <_Tp, _Mb, _MK>
{
    typedef typename list <_Tp>::iterator itItem;
    typedef typename list <itItem>::iterator tpitPos;
    private:
        int iFindOffset;

        map <_Hs, tpitPos> listFind;
        _Hs getKey(_Tp *pstDes)
        {
            return *((_Hs *)((void *)pstDes + iFindOffset));
        }
    public:
        bool setFindOffset(int iOffset)
        {
            this->iFindOffset = iOffset;
            return true;
        }
        /* ascending order */
        bool insetInMp(_Tp& __x)
        {
            tpitPos itTemp;
            _Hs stKey;

            stKey = getKey(&__x);
            if (listFind.end() == listFind.find(stKey))
            {
                itTemp = this->insetByOrder(__x);
            }
            else
            {
                itTemp = listFind[stKey];
                if (this->judgeChange(itTemp, __x))
                {
                    /* changed */
                    /* delete old */
                    this->myDel(itTemp);
                    /* insert and get new position */
                    itTemp = this->insetByOrder(__x);
                }
                else
                {
                    /* no change, ignore */
                }
            }
            listFind[stKey] = itTemp;
            return true;
        }
        _Tp findInMp(_Hs stKey)
        {
            return *(*(listFind[stKey]));
        }
        bool delInMp(_Hs stKey)
        {
            if (listFind.find(stKey) == listFind.end())
            {
                /* has been deleted */
                return false;
            }
            this->myDel(listFind[stKey]);
            listFind.erase(stKey);
            return true;
        }
        /* can't be empty */
        _Tp popInMp()
        {
            _Hs stKey;
            _Tp stDel;
            stDel = this->popInList();
            stKey = getKey(&stDel);
            listFind.erase(stKey);
            return stDel;
        }
        bool getTopBatch(list <_Hs> &lstHsKey)
        {
            list <itItem> *plsTemp;
            typename list <itItem>::iterator itlsI;
            _Tp stDel;

            plsTemp = this->getTopList();
            for (itlsI = plsTemp->begin(); itlsI != plsTemp->end(); itlsI++)
            {
                stDel = *(*itlsI);
                lstHsKey.push_back(getKey(&stDel));
            }

            return true;
        }
};








