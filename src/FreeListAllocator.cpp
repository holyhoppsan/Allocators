#include "FreeListAllocator.h"

namespace Allocators {

FreeListAllocator::FreeListAllocator(size_t Size, void* Start)
    : AllocatorBase(Size, Start),
      m_FreeBlocks(reinterpret_cast<FreeBlock*>(Start)) {
  DH_ASSERT(Size > sizeof(FreeBlock));
  m_FreeBlocks->Size = Size;
  m_FreeBlocks->Next = nullptr;
}

FreeListAllocator::~FreeListAllocator() { m_FreeBlocks = nullptr; }

void* FreeListAllocator::Allocate(size_t Size, size_t Alignment) {
  DH_ASSERT(Size != 0 && Alignment != 0);
 // FreeBlock*
  return nullptr;
}

void FreeListAllocator::Deallocate(void* Pointer) {}
}  // namespace Allocators