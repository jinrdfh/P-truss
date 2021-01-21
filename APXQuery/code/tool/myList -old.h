#pragma once
/***************
my list class
add function: insert by order
****************/
#include <stdio.h>
#include <list>

#define getStructOffset(structName, memberName) ((long)&(((structName *)0)->memberName))

template<typename _Tp, typename _Mb>
class myList : public list <_Tp>
{
    private:
        int iOffset;
        /* x < y return true */
        bool cmpFunc(_Tp *__pstX, _Tp *__pstY)
        {
            _Mb __x, __y;
            __x = *((_Mb *)((void *)__pstX + iOffset));
            __y = *((_Mb *)((void *)__pstY + iOffset));

            return (__x < __y);
        }

    public:
        bool setOffset(int iOffset)
        {
            this->iOffset = iOffset;
            return true;
        }
        bool mySort()
        {
            this->sort(cmpFunc);
        }
        /* ascending order */
        typename list<_Tp>::iterator insetByOrder(_Tp& __x)
        {
            typename list<_Tp>::iterator pstNow, pstPre, pstTemp;

            pstPre = this->begin();
            pstNow = this->begin();
            pstNow++;
            if ((pstPre != this->end()) && (pstNow == this->end()))
            {
                /* only one in list */
                if ((cmpFunc(&(*pstPre), &__x)))
                {
                    this->push_back(__x);
                    pstTemp = this->end();
                    /* get the last */
                    pstTemp--;
                    return pstTemp;
                }
                else
                {
                    this->push_front(__x);
                    return this->begin();
                }
            }
            for (; pstNow != this->end(); pstNow++, pstPre++)
            {
                if ((cmpFunc(&(*pstPre), &__x)))
                {
                    /* bigger than previous one */
                    if ((cmpFunc(&(*pstNow), &__x)))
                    {
                        /* bigger than current one, move */
                        continue;
                    }
                    else
                    {
                        /* less than current one, insert */
                        return this->insert(pstNow, __x);
                    }
                }
                else
                {
                    /* less than the first, put to the first */
                    this->push_front(__x);
                    return this->begin();
                }
            }
            if (pstNow == this->end())
            {
                this->push_back(__x);
                pstTemp = this->end();
                /* get the last */
                pstTemp--;
                return pstTemp;
            }
            return this->end();
        }
};


template<typename _Tp, typename _Mb, typename _Hs>
class myHsList : public myList <_Tp, _Mb>
{
    private:
        int iFindOffset;

        map <_Hs, typename list<_Tp>::iterator> listFind;
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
            typename list<_Tp>::iterator itTemp;
            _Hs stKey;

            stKey = getKey(&__x);
            if (listFind.end() == listFind.find(stKey))
            {
                itTemp = this->insetByOrder(__x);
            }
            else
            {
                /* delete old */
                this->erase(listFind[stKey]);
                /* insert and get new position */
                itTemp = this->insetByOrder(__x);
            }
            listFind[stKey] = itTemp;
            return true;
        }
        _Tp findInMp(_Hs stKey)
        {
            return *(listFind[stKey]);
        }
        bool delInMp(_Hs stKey)
        {
            if (listFind[stKey] == this->end())
            {
                /* has been deleted */
                return false;
            }
            this->erase(listFind[stKey]);
            listFind[stKey] = this->end();
            return true;
        }
        _Tp popInMp()
        {
            _Hs stKey;
            _Tp stDel = this->front();
            stKey = getKey(&stDel);
            this->pop_front();
            listFind[stKey] = this->end();
            return stDel;
        }
};








