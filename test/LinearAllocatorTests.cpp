#include "gtest/gtest.h"

#include "LinearAllocator.h"

struct FourByteStruct {
  char One;
  char Two;
  char Three;
  char Four;
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

// TODO: Five byte struct with a four byte aligned allocator

// TODO: Three byte struct with a four byte aligned allocator

// TODO: Five byte struct with an eight byte aligned allocator

// TODO: nullptr return when overflow

// TODO: clear resets value

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