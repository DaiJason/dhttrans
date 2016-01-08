// dhttrans.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <chrono>

#include "singleton.h"
#include "mem_pool.h"

#include <windows.h>

using namespace std;

int main()
{
    const int nLoop = 100;
    const int nCount = 10000;

    std::chrono::steady_clock::time_point tBeg = std::chrono::steady_clock::now();

    CMemTest::NewPool();
    for (int i = 0; i < nLoop; i++)
    {
        CMemTest* memT[nCount];
        for (int j = 0; j < nCount; ++j)
        {
            memT[j] = new CMemTest;
            memT[j]->m_n = j;
            memT[j]->m_n1 = j + 1;
        }
        for (int j = 0; j < nCount; ++j)
        {
            bool isRight = memT[j]->m_n == j && memT[j]->m_n1 == j + 1;
            if (!isRight)
            {
                cout << "memory error:" << j << endl;
            }
            delete memT[j];
        }
    }
    CMemTest::DeletePool();
    cout << "pool cost time : " << (std::chrono::steady_clock::now() - tBeg).count() << endl;

    tBeg = std::chrono::steady_clock::now();
    for (int i = 0; i < nLoop; i++)
    {
        CMemBaseTest* memT[nCount];
        for (int j = 0; j < nCount; ++j)
        {
            memT[j] = new CMemBaseTest;
            memT[j]->m_n = j;
            memT[j]->m_n1 = j + 1;
        }

        for (int j = 0; j < nCount; ++j)
        {
            bool isRight = memT[j]->m_n == j && memT[j]->m_n1 == j + 1;
            if (!isRight)
            {
                cout << "memory error:" << j << endl;
            }
            delete memT[j];
        }
    }
    cout << "base cost time : " << (std::chrono::steady_clock::now() - tBeg).count() << endl;
    return 0;
}

