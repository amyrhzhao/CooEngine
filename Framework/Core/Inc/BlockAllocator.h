#ifndef INCLUDED_COOENGINE_CORE_BLOCKALLOCATOR_H
#define INCLUDED_COOENGINE_CORE_BLOCKALLOCATOR_H

namespace Coo::Core
{
	class BlockAllocator
	{
	public:
		BlockAllocator(size_t blockSize, size_t capacity);
		~BlockAllocator();

		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;
		BlockAllocator(BlockAllocator&&) = delete;
		BlockAllocator& operator=(BlockAllocator&&) = delete;

		void* Allocate();
		void Free(void* ptr);

		size_t GetCapacity() const { return mCapacity; }

	protected:
		uint8_t* mData = nullptr;
		size_t mBlockSize;
		size_t mCapacity;
		// todo: write a advanced version, use next free slot. check notes
		// intptr_t mFreeSlot;
		std::vector<size_t> mFreeSlots;
	};
} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_BLOCKALLOCATOR_H
