#ifndef INCLUDED_COOENGINE_CORE_STREAMREADER_H
#define INCLUDED_COOENGINE_CORE_STREAMREADER_H

namespace Coo::Core
{

class MemoryStream;

class StreamReader
{
public:
	StreamReader(MemoryStream& memoryStream);

	template<class T>
	void Read(T& data)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic function, the argument type not supported.");
		Read(&data, sizeof(T));
	}

	void Read(std::string& data);

private:
	bool Read(void* data, uint32_t size);
	MemoryStream& mMemoryStream;
};

} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_STREAMREADER_H
