#include "Allocator.h"
#include "DHAssert.h"

namespace Allocators
{
	AllocatorBase::AllocatorBase(size_t Size, void* Start)
		: m_Start(Start)
		, m_Size(Size)
		, m_UsedMemory(0)
		, m_NumberOfAllocations(0)
	{
	}


	AllocatorBase::~AllocatorBase()
	{
		DH_ASSERT(m_NumberOfAllocations == 0 && m_UsedMemory == 0);
		m_Start = nullptr;
		m_Size = 0;
	}
}

