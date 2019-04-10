#include "LinearAllocator.h"

#include "DHAssert.h"
#include "PointerMath.h"

namespace Allocators {
LinearAllocator::LinearAllocator(size_t Size, void* Start)
    : AllocatorBase(Size, Start), m_CurrentPos(Start) {
  DH_ASSERT(Size > 0);
}

LinearAllocator::~LinearAllocator() { m_CurrentPos = nullptr; }

void* LinearAllocator::Allocate(size_t Size, size_t Alignment) {
  DH_ASSERT(Size != 0);
  size_t Adjustment =
      PointerMath::alignForwardAdjustment(m_CurrentPos, Alignment);

  if (m_UsedMemory + Adjustment + Size > m_Size) {
    return nullptr;
  }

  size_t AlignedAddress = reinterpret_cast<size_t>(m_CurrentPos) + Adjustment;
  m_CurrentPos = reinterpret_cast<void*>(AlignedAddress + Size);
  m_UsedMemory += Adjustment + Size;
  m_NumberOfAllocations++;

  return reinterpret_cast<void*>(AlignedAddress);
}

void LinearAllocator::Deallocate(void* Pointer) {
  DH_ASSERT_MESSAGE(false, "Use clear instead()");
}

void LinearAllocator::Clear() {
  m_NumberOfAllocations = 0;
  m_UsedMemory = 0;
  m_CurrentPos = m_Start;
}
}  // namespace Allocators
