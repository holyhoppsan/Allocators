#include "StackAllocator.h"

#include "PointerMath.h"

#include "DHAssert.h"

namespace Allocators {

StackAllocator::StackAllocator(size_t Size, void* Start)
    : AllocatorBase(Size, Start), m_CurrentPosition(Start) {
  DH_ASSERT(m_Size > 0);

#if _DEBUG
  m_PreviousPosition = nullptr;
#endif
}

StackAllocator::~StackAllocator() {
#if _DEBUG
  m_PreviousPosition = nullptr;
#endif

  m_CurrentPosition = nullptr;
}

void* StackAllocator::Allocate(size_t Size, size_t Alignment) {
  DH_ASSERT(m_Size != 0);
  size_t Adjustment = PointerMath::alignForwardAdjustmentWithHeader(
      m_CurrentPosition, Alignment, sizeof(AllocationHeader));

  if (m_UsedMemory + Adjustment + Size > m_Size) {
    return nullptr;
  }

  void* AlignedAddress = PointerMath::Add(m_CurrentPosition, Adjustment);

  AllocationHeader* Header = reinterpret_cast<AllocationHeader*>(
      PointerMath::Subtract(AlignedAddress, sizeof(AllocationHeader)));

  Header->Adjustment = Adjustment;
#if _DEBUG
  Header->PreviousAddress = m_PreviousPosition;
  m_PreviousPosition = AlignedAddress;
#endif

  m_CurrentPosition = PointerMath::Add(AlignedAddress, Size);
  m_UsedMemory += Size + Adjustment;
  m_NumberOfAllocations++;

  return AlignedAddress;
}

void StackAllocator::Deallocate(void* Pointer) {
  DH_ASSERT(Pointer == m_PreviousPosition);

  AllocationHeader* Header = reinterpret_cast<AllocationHeader*>(
      PointerMath::Subtract(Pointer, sizeof(AllocationHeader)));

  m_UsedMemory -= (reinterpret_cast<size_t>(m_CurrentPosition) -
                  reinterpret_cast<size_t>(Pointer)) + Header->Adjustment;

  m_CurrentPosition = PointerMath::Subtract(Pointer, Header->Adjustment);

#if _DEBUG
  m_PreviousPosition = Header->PreviousAddress;
#endif

  m_NumberOfAllocations--;
}
}  // namespace Allocators