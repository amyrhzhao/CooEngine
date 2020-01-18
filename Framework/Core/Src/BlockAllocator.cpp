#include "Precompiled.h"
#include "Debug.h"
#include "BlockAllocator.h"

Coo::Core::BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	:mBlockSize(blockSize), mCapacity(capacity)
{
	mData = new uint8_t[capacity * blockSize];
	mFreeSlots.reserve(capacity);
	for (size_t i = 0; i < capacity; ++i)
	{
		mFreeSlots.push_back(i);
	}
}

Coo::Core::BlockAllocator::~BlockAllocator()
{
	delete mData;
}

void* Coo::Core::BlockAllocator::Allocate()
{
	if (mFreeSlots.empty())
	{
		return nullptr;
	}
	else
	{
		size_t index = mFreeSlots.back();
		mFreeSlots.pop_back();
		return mData + index * mBlockSize;
	}
}

void Coo::Core::BlockAllocator::Free(void * ptr)
{
	size_t offset = static_cast<uint8_t*>(ptr) - mData;
	ASSERT(offset % mBlockSize == 0, "Pointer does not belong to this BlockAllocator");
	size_t index = offset / mBlockSize;
	ASSERT(index < mCapacity && index >= 0, "Pointer out of boundry");
	if (index < mCapacity && index >= 0)
	{
		mFreeSlots.push_back(index);
	}
}
