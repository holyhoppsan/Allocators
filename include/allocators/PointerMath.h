#pragma once

#include <bitset>
#include <iostream>

namespace PointerMath {

inline void* Add(void* Address, size_t Offset) {
  const size_t AddressTyped = reinterpret_cast<size_t>(Address);
  return reinterpret_cast<void*>(AddressTyped + Offset);
}

inline void* Subtract(void* Address, size_t Offset) {
  const size_t AddressTyped = reinterpret_cast<size_t>(Address);
  return reinterpret_cast<void*>(AddressTyped - Offset);
}

inline void* AlignForward(const void* Address, size_t Alignment) {
  const size_t AddressTyped = reinterpret_cast<size_t>(Address);
  const size_t AddressPlusAlignment =
      AddressTyped + static_cast<size_t>(Alignment - 1);
  const size_t NotAlignment = static_cast<size_t>(~(Alignment - 1));

  std::cout << "Alignment typed:             " << std::bitset<32>(AddressTyped)
            << std::endl;
  std::cout << "Alignment typed + alignment: "
            << std::bitset<32>(AddressPlusAlignment) << std::endl;
  std::cout << "Alignment Before:            " << std::bitset<32>(Alignment - 1)
            << std::endl;
  std::cout << "Not Alignment After:         " << std::bitset<32>(NotAlignment)
            << std::endl;
  std::cout << "Masked Result:               "
            << std::bitset<32>(AddressPlusAlignment & NotAlignment);
  const size_t MaskedResult = AddressPlusAlignment & NotAlignment;
  return reinterpret_cast<void*>(MaskedResult);
}

inline size_t AlignForwardAdjustment(const void* Address, size_t Alignment) {
  const size_t Adjustment =
      Alignment - (reinterpret_cast<size_t>(Address) & (Alignment - 1));

  if (Adjustment == Alignment) {
    return 0;
  }
  return Adjustment;
}

inline size_t AlignForwardAdjustmentWithHeader(const void* Address,
                                               size_t Alignment,
                                               size_t HeaderSize) {
  size_t Adjustment = AlignForwardAdjustment(Address, Alignment);
  size_t NeededSpace = HeaderSize;

  if (Adjustment < NeededSpace) {
    NeededSpace -= Adjustment;

    Adjustment += Alignment * (NeededSpace / Alignment);

    if (NeededSpace % Alignment > 0) {
      Adjustment += Alignment;
    }
  }

  return Adjustment;
}
}  // namespace PointerMath