#pragma once
#ifndef INCLUDED_COOENGINE_CORE_STREAMWRITER_H
#define INCLUDED_COOENGINE_CORE_STREAMWRITER_H

namespace Coo::Core 
{

class MemoryStream;

class StreamWriter
{
public:
	StreamWriter(MemoryStream& memoryStream);

	template<class T>
	void Write(const T& data)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic function, the argument type not supported.");
		Write(&data, sizeof(T));
	}

	void Write(const std::string& data);

private:
	void Write(const void* data, uint32_t size);
	MemoryStream& mMemoryStream;
};

} // namespace Coo::Core

#endif // !INCLUDED_COOENGINE_CORE_STREAMWRITER_H
