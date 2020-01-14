#include "Precompiled.h"
#include "StreamWriter.h"
#include "MemoryStream.h"

using namespace Coo::Core;

StreamWriter::StreamWriter(MemoryStream & memoryStream)
	: mMemoryStream{ memoryStream }
{

}

void StreamWriter::Write(const std::string& data)
{
	int stringLength = static_cast<int>(data.length());
	Write(stringLength);
	Write(data.data(), stringLength);

}

void StreamWriter::Write(const void * data, uint32_t size)
{
	uint32_t finalSize = mMemoryStream.mHead + size;
	if (finalSize > mMemoryStream.mCapacity)
	{
		mMemoryStream.ReallocBuffer(finalSize);
	}
	memcpy(mMemoryStream.mBuffer + mMemoryStream.mHead, data, size);
	mMemoryStream.mHead += size;
}
