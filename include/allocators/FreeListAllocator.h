#pragma once

#include <cstdint>

#include "Allocator.h"

namespace Allocators {
class FreeListAllocator : public AllocatorBase {
 public:
  FreeListAllocator(size_t Size, void* Start);
  FreeListAllocator(const FreeListAllocator& Allocator) = delete;
  FreeListAllocator& operator=(const FreeListAllocator& Allocator) = delete;

  virtual ~FreeListAllocator();

  void* Allocate(size_t Size, size_t Alignment) override;
  void Deallocate(void* Pointer) override;

 private:
  struct AllocationHeader {
    size_t Size;
    size_t Adjustment;
  };

  struct FreeBlock {
    size_t Size;
    FreeBlock* Next;
  };

  FreeBlock* m_FreeBlocks;
};
}  // namespace Allocators
