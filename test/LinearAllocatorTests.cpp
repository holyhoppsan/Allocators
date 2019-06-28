#include "gtest/gtest.h"

#include "LinearAllocator.h"

struct FourByteStruct {
  char One;
  char Two;
  char Three;
  char Four;
};

TEST(linearallocator, allocator_size_is_initialized_correctly) {
  const int32_t Size = 1024;
  void* Memory = malloc(Size);

  Allocators::LinearAllocator Allocator(Size, Memory);

  ASSERT_EQ(Allocator.GetSize(), Size);

  free(Memory);
}

TEST(linearallocator, allocator_start_pointer_is_initialized_correctly) {
  const int32_t Size = 1024;
  void* Memory = malloc(Size);

  Allocators::LinearAllocator Allocator(Size, Memory);

  ASSERT_TRUE(Allocator.GetStart() != nullptr);

  free(Memory);
}

TEST(linearallocator, allocator_used_memory_is_zero_after_creation) {
  const int32_t Size = 1024;
  void* Memory = malloc(Size);

  Allocators::LinearAllocator Allocator(Size, Memory);

  ASSERT_EQ(Allocator.GetUsedMemory(), 0);

  free(Memory);
}

TEST(linearallocator, allocator_number_of_allocations_is_zero_after_creation) {
  const int32_t Size = 1024;
  void* Memory = malloc(Size);

  Allocators::LinearAllocator Allocator(Size, Memory);

  ASSERT_EQ(Allocator.GetUsedMemory(), 0);

  free(Memory);
}

TEST(
    linearallocator,
    allocating_a_four_byte_struct_yields_correct_allocation_for_a_four_byte_aligned_allocator) {
  const int32_t Size = 1024;
  void* Memory = malloc(Size);

  Allocators::LinearAllocator Allocator(Size, Memory);

  FourByteStruct* AllocatedPtr =
      Allocators::AllocateNew<FourByteStruct>(Allocator);

  ASSERT_EQ(Allocator.GetSize(), Size);
  ASSERT_TRUE(Allocator.GetStart() != nullptr);
  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 1);
  ASSERT_EQ(Allocator.GetUsedMemory(), 4);

  Allocator.Clear();
  free(Memory);
}

TEST(
    linearallocator,
    allocating_two_four_byte_struct_yields_correct_allocation_for_a_four_byte_aligned_allocator) {
  const int32_t Size = 1024;
  void* Memory = malloc(Size);

  Allocators::LinearAllocator Allocator(Size, Memory);

  FourByteStruct* FirstAllocatedPtr =
      Allocators::AllocateNew<FourByteStruct>(Allocator);

  FourByteStruct* SecondAllocatedPtr =
      Allocators::AllocateNew<FourByteStruct>(Allocator);

  ASSERT_EQ(Allocator.GetSize(), Size);
  ASSERT_TRUE(Allocator.GetStart() != nullptr);
  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 2);
  ASSERT_EQ(Allocator.GetUsedMemory(), 8);

  ASSERT_EQ(reinterpret_cast<size_t>(SecondAllocatedPtr) -
                reinterpret_cast<size_t>(Allocator.GetStart()),
            4);

  Allocator.Clear();
  free(Memory);
}

// TODO: Five byte struct with a four byte aligned allocator

// TODO: Three byte struct with a four byte aligned allocator

// TODO: Five byte struct with an eight byte aligned allocator

// TODO: nullptr return when overflow

// TODO: clear resets value

// TODO if DH_ASSERT is caught by the test runner: Deallocating asserts

// TODO: Array allocation correctly with different alignments

// TODO: Validate constructor is invoked for single allocation

// TODO: Validate that constructor is called for array allocations