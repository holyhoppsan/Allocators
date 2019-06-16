#pragma once

#include <cstdint>

#include "Allocator.h"

namespace Allocators {
class StackAllocator : public AllocatorBase {
 public:
  StackAllocator(size_t Size, void* Start);
  StackAllocator(const StackAllocator& Allocator) = delete;
  StackAllocator& operator=(const StackAllocator& Allocator) = delete;

  virtual ~StackAllocator();

  void* Allocate(size_t Size, size_t Alignment) override;
  void Deallocate(void* Pointer) override;

 private:
  struct AllocationHeader {
#ifdef _DEBUG
    void* PreviousAddress;
#endif
    size_t Adjustment;
  };

#if _DEBUG
  void* m_PreviousPosition;
#endif

  void* m_CurrentPosition;
};
}  // namespace Allocators