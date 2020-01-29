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
		Handle<DataType>::sPool = this;
		mEntries.resize(capacity + 1);
		mFreeSlots.reserve(capacity);
		for (size_t i = capacity; i > 0; --i)
		{
			mFreeSlots.push_back(i);
		}
	}

	template<class DataType>
	inline HandlePool<DataType>::~HandlePool()
	{
		for (auto& entry : mEntries)
		{
			ASSERT(entry.instance == nullptr, "There is still live handle left in handle pool.");
		}
	}

	template<class DataType>
	inline Handle<DataType> HandlePool<DataType>::Register(DataType* instance)
	{
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
			entry.instance = nullptr;
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
