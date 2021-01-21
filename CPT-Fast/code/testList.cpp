#include "tool/myList.h"
typedef struct TestList
{
    float fKey;
    pair <int, int> paKey;
    int iValue;
}TEST_LIST;

int main()
{
    TEST_LIST a, b, c, d, e, test;
    myHsList<TEST_LIST, float, pair<int, int> > lstTest;
    a.fKey = 1;
    b.fKey = 3;
    c.fKey = 2;
    d.fKey = 6;
    e.fKey = 0;
    a.paKey = pair<int, int>(1,3);
    b.paKey = pair<int, int>(2,3);
    c.paKey = pair<int, int>(3,3);
    d.paKey = pair<int, int>(1,2);
    e.paKey = pair<int, int>(1,1);

    lstTest.setOrderOffset(getStructOffset(TEST_LIST, fKey));
    lstTest.setFindOffset(getStructOffset(TEST_LIST, paKey));

    lstTest.insetInMp(a);
    lstTest.insetInMp(b);
    lstTest.insetInMp(c);
    lstTest.insetInMp(d);
    lstTest.insetInMp(e);

    while(!lstTest.empty())
    {
        test = lstTest.popInMp();
        printf("f: %f pair: (%d, %d)\n",
               test.fKey, test.paKey.first, test.paKey.second);
    }

    return 0;
}
