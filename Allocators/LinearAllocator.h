#pragma once

#include <cstdint>

#include "Allocator.h"

namespace Allocators {
class LinearAllocator : public AllocatorBase {
 public:
  LinearAllocator(size_t Size, void* Start);

  LinearAllocator(const LinearAllocator& Allocator) = delete;
  LinearAllocator(LinearAllocator&& Allocator) = delete;

  virtual ~LinearAllocator();

  LinearAllocator& operator=(const LinearAllocator& Allocator) = delete;

  virtual void* Allocate(size_t Size, size_t Alignment) override;
  virtual void Deallocate(void* Pointer) override;

  void Clear();

 private:
  void* m_CurrentPos;
};
}  // namespace Allocators
