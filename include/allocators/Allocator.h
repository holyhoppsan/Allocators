#pragma once

#include <cstdint>

#include "DHAssert.h"

namespace Allocators {
class AllocatorBase {
 public:
  AllocatorBase(size_t Size, void* Start);
  virtual ~AllocatorBase();

  virtual void* Allocate(size_t Size, size_t Alignment = 4) = 0;
  virtual void Deallocate(void* Pointer) = 0;

  void* GetStart() const { return m_Start; }
  size_t GetSize() const { return m_Size; }
  size_t GetUsedMemory() const { return m_UsedMemory; }
  size_t GetNumberOfAllocations() const { return m_NumberOfAllocations; }

 protected:
  void* m_Start;
  size_t m_Size;
  size_t m_UsedMemory;
  size_t m_NumberOfAllocations;
};

template <typename T>
T* AllocateNew(AllocatorBase& Allocator) {
  return new (Allocator.Allocate(sizeof(T), alignof(T))) T;
}

template <typename T>
T* AllocateNew(AllocatorBase& Allocator, const T& t) {
  return new (Allocator.Allocate(sizeof(T), alignof(T))) T(t);
}

template <typename T, typename... Args>
T* AllocateNew(AllocatorBase& Allocator, Args&&... Arguments) {
  return new (Allocator.Allocate(sizeof(T), alignof(T)))
      T(std::forward<Args>(Arguments)...);
}

template <typename T>
void DeallocateDelete(AllocatorBase& Allocator, T& Object) {
  Object.~T();
  Allocator.Deallocate(&Object);
}

template <typename T>
T* AllocateArray(AllocatorBase& Allocator, size_t Length) {
  DH_ASSERT(Length != 0);
  uint8_t HeaderSize = sizeof(size_t) / sizeof(T);

  if (sizeof(size_t) % sizeof(T) > 0) {
    HeaderSize += 1;
  }

  T* p = (Allocator.Allocate(sizeof(T) * (Length + HeaderSize), alignof(T))) +
         HeaderSize;
  *((reinterpret_cast<size_t*>(p)) - 1) = Length;

  for (size_t i = 0; i < Length; i++) {
    new (&p) T;
  }

  return p;
}

template <typename T>
void DeallocateArray(AllocatorBase& Allocator, T* Array) {
  DH_ASSERT(Array != nullptr);
  size_t Length = *(reinterpret_cast<size_t*>(Array) - 1);

  for (size_t i = 0; i < Length; i++) {
    Array[i].~T();
  }

  uint8_t HeaderSize = sizeof(size_t) / sizeof(T);

  if (sizeof(size_t) % sizeof(T) > 0) {
    HeaderSize += 1;
  }

  Allocator.Deallocate(Array - HeaderSize);
}
}  // namespace Allocators
