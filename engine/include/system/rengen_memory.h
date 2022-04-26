#ifndef RENGEN_MEMORY_H_
#define RENGEN_MEMORY_H_

#include "common.h"
#include <memory>
#include <malloc/malloc.h>
#include <vector>

#ifndef L1_CACHE_LINE_SIZE
#define L1_CACHE_LINE_SIZE 64
#endif

namespace rengen
{
    class COREDLL ReferenceCounted
    {
    public:
        ReferenceCounted() { nReferences = 0; }
        int nReferences;

    private:
        ReferenceCounted(const ReferenceCounted &);
        ReferenceCounted &operator=(const ReferenceCounted &);
    };

    template <class T>
    class Reference
    {
    public:
        // Initial Reference Container setup
        Reference(T *p = nullptr)
        {
            m_ptr = p;
            if (m_ptr)
                ++(m_ptr->nReferences);
        }

        // Copy Construct, adds a refcount to the underlying object
        Reference(const Reference<T> &r)
        {
            m_ptr = r.m_ptr;
            if (m_ptr)
                ++(m_ptr->nReferences);
        }

        // Assigment operator, we need to decrement the ref count on the old object, and increment on the new object
        Reference &operator=(const Reference<T> &r)
        {
            // Increment the refcount on the incomming object
            if (r.m_ptr)
                (r.m_ptr->nReferences)++;

            // Decrement the old refcount -- if we hit zero, free the memory as no one else is using it...
            if (m_ptr && --(m_ptr->nReferences) == 0)
                delete m_ptr;

            // Make the new assignment
            m_ptr = r.m_ptr;

            return (*this);
        }

        // Assignment to a bare pointer of a refcounted object
        Reference &operator=(T *p)
        {
            if (p)
                p->nReferences++;

            if (m_ptr && --(m_ptr->nReferences) == 0)
                delete m_ptr;

            m_ptr = p;
            return *this;
        }

        ~Reference()
        {
            // Decrement refcount and check for 0 refs -- delete if we are the last reference
            if (m_ptr && --(m_ptr->nReferences) == 0)
                delete m_ptr;
        }

        T *operator->() { return m_ptr; }
        const T *operator->() const { return m_ptr; }
        operator bool() const { return m_ptr != nullptr; }
        bool operator<(const Reference<T> &r2) const
        {
            return m_ptr < r2.m_ptr;
        }

    private:
        T *m_ptr;
    };

    // Cache Line Size Aligned Memory Allocation
    COREDLL void *AllocAligned(size_t size);

    // Free Aligned Memorh Pointer
    COREDLL void FreeAligned(void *ptr);

    // Object Arenas

    template <class T>
    class ObjectArena
    {
    public:
        ObjectArena() : nAvailable(0) {}

        T *Alloc()
        {
            if (nAvailable == 0)
            {
                int nAlloc = fmax((unsigned long)16, (unsigned long)(65536 / sizeof(T)));
                mem = (T *)AllocAligned(nAlloc * sizeof(T));
                nAvailable = nAlloc;
                toDelete.push_back(mem);
            }
            --nAvailable;
            return mem++;
        }

        operator T *()
        {
            return Alloc();
        }

        void FreeAll()
        {
            for (uint64_t i = 0; i < toDelete.size(); i++)
            {
                FreeAligned(toDelete[i]);
            }
            toDelete.erase(toDelete.begin(), toDelete.end());
            nAvailable = 0;
        }

    private:
        T *mem;
        int nAvailable;
        std::vector<T *> toDelete;
    };

    class MemoryArena
    {
    public:
        MemoryArena(uint64_t bs = 32768)
        {
            m_blockSize = bs;
            m_curBlockPos = 0;
            m_currentBlock = (char *)AllocAligned(m_blockSize);
        }

        void *Alloc(uint64_t sz)
        {
            sz = ((sz + 7) & (~7));
            if (m_curBlockPos + sz > m_blockSize)
            {
                m_usedBlocks.push_back(m_currentBlock);
                if (m_availableBlocks.size() && sz <= m_blockSize)
                {
                    m_currentBlock = m_availableBlocks.back();
                    m_availableBlocks.pop_back();
                }
                else
                {
                    m_currentBlock = (char *)AllocAligned(fmax(sz, m_blockSize));
                }
                m_curBlockPos = 0;
            }
            void *ret = m_currentBlock + m_curBlockPos;
            m_curBlockPos += sz;
            return ret;
        }

        void FreeAll()
        {
            m_curBlockPos = 0;
            while (m_usedBlocks.size())
            {
                m_availableBlocks.push_back(m_usedBlocks.back());
                m_usedBlocks.pop_back();
            }
        }

    private:
        uint64_t m_blockSize;
        uint64_t m_curBlockPos;
        char *m_currentBlock;
        std::vector<char *> m_usedBlocks, m_availableBlocks;
    };

    template <class T, int logBlockSize>
    class BlockedArray
    {
    public:
        BlockedArray(int nu, int nv, const T *d = NULL)
        {
            uRes = nu;
            vRes = nv;
            uBlocks = RoundUp(uRes) >> logBlockSize;
            int nAlloc = RoundUp(uRes) * RoundUp(vRes);
            data = (T *)AllocAligned(nAlloc * sizeof(T));
            for (int i = 0; i < nAlloc; i++)
            {
                new (&data[i]) T();
            }

            if (d)
                for (int v = 0; v < nv; ++v)
                    for (int u = 0; u < nu; ++u)
                        (*this)(u, v) = d[v * uRes + u];
        }

        int uSize() const { return uRes; }
        int vSize() const { return vRes; }

        int BlockSize() const { return 1 << logBlockSize; }
        int RoundUp(int x) const { return (x + BlockSize() - 1) & ~(BlockSize() - 1); }

        int Block(int a) const { return a >> logBlockSize; }
        int Offset(int a) const { return (a & (BlockSize() - 1)); }

        T &operator()(int u, int v)
        {
            int bu = Block(u), bv = Block(v);
            int ou = Offset(u), ov = Offset(v);
            int offset = BlockSize() * BlockSize() * (uBlocks * bv + bu);
            offset += BlockSize() * ov + ou;
            return data[offset];
        }

        void GetLinearArray(T *a) const
        {
            for (int v = 0; v < vRes; ++v)
                for (int u = 0; u < uRes; ++u)
                    *a++ = (*this)(u, v);
        }

    private:
        int uRes, vRes, uBlocks;
        T *data;
    };
}
#endif