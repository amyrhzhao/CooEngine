#ifndef INCLUDED_COOENGINE_CORE_HANDLE_H
#define INCLUDED_COOENGINE_CORE_HANDLE_H

namespace Coo::Core
{
	template <class DataType>
	class HandlePool;

	template <class DataType>
	class Handle
	{
	public:
		Handle();

		bool IsValid() const;
		void Invalidate();

		DataType* Get() const;
		DataType* operator->() const;

		bool operator==(Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }
		bool operator!=(Handle rhs) const { return !(*this == rhs); }

	private:
		friend class HandlePool<DataType>;

		static HandlePool<DataType>* sPool;

		uint32_t mIndex : 16;
		uint32_t mGeneration : 16;
	};

	template <class DataType>
	HandlePool<DataType>* Handle<DataType>::sPool = nullptr;

	template<class DataType>
	inline Handle<DataType>::Handle()
		: mIndex(0)
		, mGeneration(0)
	{

	}

	template<class DataType>
	inline bool Handle<DataType>::IsValid() const
	{
		return sPool && sPool->IsValid(*this);
	}

	template<class DataType>
	inline void Handle<DataType>::Invalidate()
	{
		*this = Handle();
	}

	template<class DataType>
	inline DataType* Handle<DataType>::Get() const
	{
		return sPool ? sPool->Get(*this) : nullptr;
	}

	template<class DataType>
	inline DataType* Handle<DataType>::operator->() const
	{
		return sPool ? sPool->Get(*this) : nullptr;
	}

} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_HANDLE_H
