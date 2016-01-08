/*
*	内存池
*	Author:daihongtian
*	说明:适用于内存频繁分配释放场景
*	本机测试能比普通的快15~20倍左右
*/

#pragma once

//单线程内存池
template<typename T>
class CMemPool
{
public:
    enum { EXPANSION_SIZE = 32 };

    CMemPool(unsigned int nItemCount = EXPANSION_SIZE)
    {
        ExpandFreeList(nItemCount);
    }

    ~CMemPool()
    {
        CMemPool<T>* pNext = NULL;
        for (pNext = m_pFreeList; pNext != NULL; pNext = m_pFreeList)
        {
            m_pFreeList = m_pFreeList->m_pFreeList;
            delete[](char*)pNext;
        }
    }

    void* Alloc(unsigned int /*size*/)
    {
        if (m_pFreeList == NULL)
        {
            ExpandFreeList();
        }

        CMemPool<T>* pHead = m_pFreeList;
        m_pFreeList = m_pFreeList->m_pFreeList;
        return pHead;
    }

    void Free(void* p)
    {
        CMemPool<T>* pHead = static_cast<CMemPool<T>*>(p);
        pHead->m_pFreeList = m_pFreeList;
        m_pFreeList = pHead;
    }

protected:
    //allocate memory and push to the list
    void ExpandFreeList(unsigned nItemCount = EXPANSION_SIZE)
    {
        unsigned int nSize = sizeof(T) > sizeof(CMemPool<T>) ? sizeof(T) : sizeof(CMemPool<T>*);
        CMemPool<T>* pLastItem = static_cast<CMemPool<T>*>(static_cast<void*>(new char[nSize]));
        m_pFreeList = pLastItem;
        for (unsigned int i = 0; i < nItemCount - 1; ++i)
        {
            pLastItem->m_pFreeList = static_cast<CMemPool<T>*>(static_cast<void*>(new char[nSize]));
            pLastItem = pLastItem->m_pFreeList;
        }

        pLastItem->m_pFreeList = NULL;
    }

private:
    CMemPool<T>* m_pFreeList;
};

//多线程内存池
template<typename T>
class CMTMemPool
{
public:
    void* Alloc(unsigned int size)
    {
        void* p = NULL;
        std::lock_guard lock(m_lock);
        p = m_pool.Alloc(size);
        return p;
    }

    void Free(void* p)
    {
        std::lock_guard lock(m_lock);
        m_pool.Free(p);
    }

private:
    CMemPool<T>	m_pool;
    std::mutex		m_lock;
};


class CMemTest
{
public:
    int m_n;
    int m_n1;

    void* operator new(size_t size)
    {
        void* p = s_pool->Alloc(size);
        return p;
    }

    void operator delete(void* p, size_t size)
    {
        s_pool->Free(p);
    }

    static void NewPool()
    {
        s_pool = new CMemPool<CMemTest>;
    }

    static void DeletePool()
    {
        delete s_pool;
        s_pool = 0;
    }

    static CMemPool<CMemTest>* s_pool;
};

CMemPool<CMemTest>* CMemTest::s_pool = 0;

class CMemBaseTest {
public:
    int m_n;
    int m_n1;
};