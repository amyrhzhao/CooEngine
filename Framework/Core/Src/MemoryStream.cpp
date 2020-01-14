#include "Precompiled.h"
#include "MemoryStream.h"

using namespace Coo::Core;

MemoryStream::MemoryStream(uint32_t capacity)
{
	ReallocBuffer(capacity);
}

MemoryStream::MemoryStream(const uint8_t * data, uint32_t size)
	:mCapacity{ size }
{
	mBuffer = static_cast<uint8_t*>(std::malloc(size));
	memcpy(mBuffer, data, size);
}

MemoryStream::~MemoryStream()
{
	std::free(mBuffer);
}

void MemoryStream::Rewind()
{
	mHead = 0;
}

void MemoryStream::ReallocBuffer(uint32_t newCapacity)
{
	mBuffer = static_cast<uint8_t*>(std::realloc(mBuffer, newCapacity));
	mCapacity = newCapacity;
}