#ifndef INCLUDED_COOENGINE_CORE_HANDLEPOOL_H
#define INCLUDED_COOENGINE_CORE_HANDLEPOOL_H

namespace Coo::Core
{
	template<class DataType>
	class Handle;

	template<class DataType>
	class HandlePool
	{
	public:
		using HandleType = Handle<DataType>;
		HandlePool(size_t capacity);
		~HandlePool();

		HandleType Register(DataType* instance);
		void Unregister(HandleType handle);
		void Flush();

		bool IsValid(HandleType handle) const;
		DataType* Get(HandleType handle);

	private:
		struct Entry
		{
			DataType* instance = nullptr;
			size_t generation = 0;
		};

		std::vector<Entry> mEntries;
		std::vector<size_t> mFreeSlots;
	};

	template<class DataType>
	inline HandlePool<DataType>::HandlePool(size_t capacity)
	{
		ASSERT(capacity > 0, "Invalid capacity!");
		mEntries.resize(capacity + 1);
		mFreeSlots.reserve(capacity);
		for (size_t i = capacity; i > 0; --i)
		{
			mFreeSlots.push_back(i);
		}

		// Register itself with the handle type
		ASSERT(HandleType::sPool == nullptr, "Cannot have more than one pool for the same type!");
		Handle<DataType>::sPool = this;
	}

	template<class DataType>
	inline HandlePool<DataType>::~HandlePool()
	{
		// Check thar all handles are freed
		ASSERT(mFreeSlots.size() == mFreeSlots.capacity(), "There is still live handle left in handle pool.");

		// Unregister itself with the handle type
		ASSERT(HandleType::sPool == this, "Something is wrong");
		HandleType::sPool = nullptr;
	}

	template<class DataType>
	inline Handle<DataType> HandlePool<DataType>::Register(DataType* instance)
	{
		ASSERT(instance != nullptr, "Handle Pointer cannot be nulllptr.");
		ASSERT(!mFreeSlots.empty(), "No more free slots.");

		Handle<DataType> newHandle;
		if (!mFreeSlots.empty())
		{
			size_t slot = mFreeSlots.back();
			mFreeSlots.pop_back();

			auto& entry = mEntries[slot];
			entry.instance = instance;

			newHandle.mIndex = slot;
			newHandle.mGeneration = entry.generation;
		}
		return newHandle;
	}

	template<class DataType>
	inline void HandlePool<DataType>::Unregister(HandleType handle)
	{
		auto& entry = mEntries[handle.mIndex];
		entry.generation++;
		entry.instance = nullptr;
	}

	template<class DataType>
	inline void HandlePool<DataType>::Flush()
	{
		// Force increment generation and invalidate all existing handles
		for (auto& entry : mEntries) 
		{
			entry.generation++;
		}

		// Reclaim all slots
		mFreeSlots.clear();
		for (size_t i = mFreeSlots.capacity(); i > 0; --i)
		{
			mFreeSlots.push_back(i);
		}
	}

	template<class DataType>
	inline bool HandlePool<DataType>::IsValid(HandleType handle) const
	{
		return handle.mGeneration == mEntries[handle.mIndex].generation && mEntries[handle.mIndex].instance;
	}

	template<class DataType>
	inline DataType* HandlePool<DataType>::Get(HandleType handle)
	{
		if (IsValid(handle))
		{
			return mEntries[handle.mIndex].instance;
		}
		else
		{
			return nullptr;
		}
	}

} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_HANDLEPOOL_H
