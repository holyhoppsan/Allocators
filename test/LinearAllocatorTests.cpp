#include "gtest/gtest.h"

#include "LinearAllocator.h"

struct FourByteStruct {
  char One;
  char Two;
  char Three;
  char Four;
};

struct FiveByteStruct {
  char One;
  int32_t Two;
};

class LinearAllocatorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    BlobSize = 1024;
    Memory = malloc(BlobSize);
  }

  void TearDown() override { free(Memory); }

  size_t BlobSize;
  void* Memory;
};

TEST_F(LinearAllocatorTest, allocator_size_is_initialized_correctly) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  ASSERT_EQ(Allocator.GetSize(), BlobSize);
}

TEST_F(LinearAllocatorTest, allocator_start_pointer_is_initialized_correctly) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  ASSERT_TRUE(Allocator.GetStart() != nullptr);
}

TEST_F(LinearAllocatorTest, allocator_used_memory_is_zero_after_creation) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  ASSERT_EQ(Allocator.GetUsedMemory(), 0);
}

TEST_F(LinearAllocatorTest,
       allocator_number_of_allocations_is_zero_after_creation) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  ASSERT_EQ(Allocator.GetUsedMemory(), 0);
}

TEST_F(
    LinearAllocatorTest,
    allocating_a_four_byte_struct_yields_correct_allocation_for_a_four_byte_aligned_allocator) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  FourByteStruct* AllocatedPtr =
      Allocators::AllocateNew<FourByteStruct>(Allocator);

  ASSERT_EQ(Allocator.GetSize(), BlobSize);
  ASSERT_TRUE(Allocator.GetStart() != nullptr);
  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 1);
  ASSERT_EQ(Allocator.GetUsedMemory(), 4);

  Allocator.Clear();
}

TEST_F(
    LinearAllocatorTest,
    allocating_two_four_byte_struct_yields_correct_allocation_for_a_four_byte_aligned_allocator) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  FourByteStruct* FirstAllocatedPtr =
      Allocators::AllocateNew<FourByteStruct>(Allocator);

  FourByteStruct* SecondAllocatedPtr =
      Allocators::AllocateNew<FourByteStruct>(Allocator);

  ASSERT_EQ(Allocator.GetSize(), BlobSize);
  ASSERT_TRUE(Allocator.GetStart() != nullptr);
  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 2);
  ASSERT_EQ(Allocator.GetUsedMemory(), 8);

  ASSERT_EQ(reinterpret_cast<size_t>(SecondAllocatedPtr) -
                reinterpret_cast<size_t>(Allocator.GetStart()),
            4);

  Allocator.Clear();
}

// TODO: three char struct

TEST_F(
    LinearAllocatorTest,
    allocating_a_four_byte_aligned_object_after_an_unaligned_allocation_adjusts_for_alignment) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  char* BytePtr = Allocators::AllocateNew<char>(Allocator);

  ASSERT_EQ(Allocator.GetSize(), BlobSize);
  ASSERT_TRUE(Allocator.GetStart() != nullptr);
  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 1);
  ASSERT_EQ(Allocator.GetUsedMemory(), 1);

  int* FourBytePtr = Allocators::AllocateNew<int>(Allocator);

  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 2);
  ASSERT_EQ(Allocator.GetUsedMemory(), 8);

  ASSERT_EQ(reinterpret_cast<size_t>(FourBytePtr) -
                reinterpret_cast<size_t>(Allocator.GetStart()),
            4);

  Allocator.Clear();
}

TEST_F(
    LinearAllocatorTest,
    allocating_a_five_byte_struct_yields_correct_allocation_for_a_four_byte_aligned_allocator) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  FiveByteStruct* AllocatedPtr =
      Allocators::AllocateNew<FiveByteStruct>(Allocator);

  ASSERT_EQ(sizeof(FiveByteStruct), 8);
  ASSERT_EQ(Allocator.GetSize(), BlobSize);
  ASSERT_TRUE(Allocator.GetStart() != nullptr);
  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 1);
  ASSERT_EQ(Allocator.GetUsedMemory(), 8);

  FiveByteStruct* SecondAllocatedPtr =
      Allocators::AllocateNew<FiveByteStruct>(Allocator);

  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 2);
  ASSERT_EQ(Allocator.GetUsedMemory(), 16);

  ASSERT_EQ(reinterpret_cast<size_t>(SecondAllocatedPtr) -
                reinterpret_cast<size_t>(Allocator.GetStart()),
            8);

  Allocator.Clear();
}

TEST_F(LinearAllocatorTest, allocation_returns_nullptr_when_overflow_occur) {
  const uint32_t Size = 2;
  Allocators::LinearAllocator Allocator(Size, Memory);

  FiveByteStruct* AllocatedPtr =
      Allocators::AllocateNew<FiveByteStruct>(Allocator);

  ASSERT_EQ(AllocatedPtr, nullptr);
}

// TODO: clear resets value

TEST_F(LinearAllocatorTest, clear_resets_the_allocator) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  FiveByteStruct* AllocatedPtr =
      Allocators::AllocateNew<FiveByteStruct>(Allocator);

  ASSERT_EQ(sizeof(FiveByteStruct), 8);
  ASSERT_EQ(Allocator.GetSize(), BlobSize);
  ASSERT_TRUE(Allocator.GetStart() != nullptr);
  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 1);
  ASSERT_EQ(Allocator.GetUsedMemory(), 8);

  Allocator.Clear();

  ASSERT_EQ(Allocator.GetSize(), BlobSize);
  ASSERT_TRUE(Allocator.GetStart() != nullptr);
  ASSERT_EQ(Allocator.GetNumberOfAllocations(), 0);
  ASSERT_EQ(Allocator.GetUsedMemory(), 0);
}

#ifdef _DEBUG
TEST_F(LinearAllocatorTest, deallocating_leads_to_an_assert) {
  Allocators::LinearAllocator Allocator(BlobSize, Memory);

  FourByteStruct* AllocatedPtr =
      Allocators::AllocateNew<FourByteStruct>(Allocator);

  ASSERT_DEATH(Allocators::DeallocateDelete(Allocator, AllocatedPtr),
               ".*Use\\sclear\\sinstead.*");

  Allocator.Clear();
}
#endif  // DEBUG

// TODO: Array allocation correctly with different alignments

// TODO: Validate constructor is invoked for single allocation

// TODO: Validate that constructor is called for array allocations