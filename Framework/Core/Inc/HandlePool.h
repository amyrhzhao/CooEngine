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
		mFreeSlots.resize(capacity);
		for (size_t i = 1; i < mFreeSlots.size() + 1; ++i)
		{
			mFreeSlots[i] = i;
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
		entry.mGeneration++;
		entry.instance = nullptr;
	}

	template<class DataType>
	inline bool HandlePool<DataType>::IsValid(HandleType handle) const
	{
		return handle.mGeneration == mEntries[handle.mIndex].generation;
	}

	template<class DataType>
	inline DataType* HandlePool<DataType>::Get(HandleType handle)
	{
		if (IsValid(handle))
		{
			return mEntries[handle.mIndex];
		}
		else
		{
			return nullptr;
		}
	}

} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_HANDLEPOOL_H
