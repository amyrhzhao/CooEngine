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
		HandlePool(uint32_t capacity);
		~HandlePool();

		HandleType Register(DataType* instance);
		void Unregister(HandleType handle);

		bool IsValid(HandleType handle) const;
		DataType* Get(HandleType handle);

	private:
		struct Entry
		{
			DataType* instance;
			uint32_t generation;
		};

		std::vector<Entry> mEntries;
		std::vector<uint32_t> mFreeSlots;
	};

	template<class DataType>
	inline HandlePool<DataType>::HandlePool(uint32_t capacity)
	{
		mEntries.resize(capacity + 1);
		mFreeSlots.resize(capacity);
		for (size_t i = 0; i < mFreeSlots.size(); ++i) 
		{
			mFreeSlots[i] = i + 1;
		}
	}

	template<class DataType>
	inline HandlePool<DataType>::~HandlePool()
	{
		//???
	}

	template<class DataType>
	inline Handle<DataType> HandlePool<DataType>::Register(DataType* instance)
	{
		auto slot = mFreeSlots.back();
		mFreeSlots.pop_back();

		return HandleType();
	}

	template<class DataType>
	inline void HandlePool<DataType>::Unregister(HandleType handle)
	{
	}

	template<class DataType>
	inline bool HandlePool<DataType>::IsValid(HandleType handle) const
	{
		return false;
	}

	template<class DataType>
	inline DataType* HandlePool<DataType>::Get(HandleType handle)
	{
		return NULL;
	}

} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_HANDLEPOOL_H
