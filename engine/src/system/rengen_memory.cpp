#include "system/rengen_memory.h"

namespace rengen {
// Cache Line Size Aligned Memory Allocation
COREDLL void *AllocAligned(size_t size) {
  void *ptr = NULL;
  if (posix_memalign(&ptr, L1_CACHE_LINE_SIZE, size))
    ptr = NULL;

  return ptr;
}

// Free Aligned Memorh Pointer
COREDLL void FreeAligned(void *ptr) { free(ptr); }
} // namespace rengen