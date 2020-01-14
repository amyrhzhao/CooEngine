#include "Precompiled.h"
#include "StreamReader.h"
#include "MemoryStream.h"

using namespace Coo::Core;

StreamReader::StreamReader(MemoryStream & memoryStream)
	: mMemoryStream{ memoryStream }
{
	memoryStream.Rewind();
}

void StreamReader::Read(std::string & data)
{
	int stringLength;
	Read(stringLength);
	char* str = new char[stringLength];
	Read(str, stringLength);
	data.assign(str, stringLength);
}

bool StreamReader::Read(void * data, uint32_t size)
{
	if (mMemoryStream.mHead + size > mMemoryStream.mCapacity)
	{
		return false;
	}
	memcpy(data, mMemoryStream.mBuffer + mMemoryStream.mHead, size);
	mMemoryStream.mHead += size;
	return true;
}
