#pragma once

#include <cstdint>

#include "DHAssert.h"

namespace Allocators
{
	class AllocatorBase
	{
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
	T* allocateNew(AllocatorBase& Allocator)
	{
		return new (Allocator.Allocate(sizeof(T), alignof(T))) T;
	}

	template <typename T>
	T* allocateNew(AllocatorBase& Allocator, const T& t)
	{
		return new (Allocator.Allocate(sizeof(T), _alignof(T))) T(t);
	}

	template <typename T>
	void deallocateDelete(AllocatorBase& Allocator, T& Object)
	{
		Object.~T();
		Allocator.Deallocate(&Object);
	}

	template <typename T>
	T* allocateArray(AllocatorBase& Allocator, size_t Length)
	{
		DH_ASSERT(Length != 0);
		uint8_t headerSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0)
		{
			headerSize += 1;
		}

		T* p = (Allocator.Allocate(sizeof(T) * (Length + headerSize), alignof(T))) + headerSize;
		*((reinterpret_cast<size_t*>(p)) - 1) = Length;

		for (size_t i = 0; i < Length; i++)
		{
			new(&p) T;
		}

		return p;
	}

	template <typename T>
	void deallocateArray(AllocatorBase& Allocator, T* Array)
	{
		DH_ASSERT(Array != nullptr);
		size_t Length = *(reinterpret_cast<size_t*>(Array) - 1);

		for (size_t i = 0; i < Length; i++)
		{
			Array[i].~T();
		}

		uint8_t HeaderSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0)
		{
			HeaderSize += 1;
		}

		Allocator.Deallocate(Array - HeaderSize);
	}
}
