#ifndef INCLUDED_COOENGINE_GRAPHICS_CONSTANTBUFFER_H
#define INCLUDED_COOENGINE_GRAPHICS_CONSTANTBUFFER_H

namespace Coo::Graphics {

class ConstantBuffer
{
public:
	void Initialize(uint32_t constantSize, uint32_t numConstants);
	void Initialize(uint32_t bufferSize, const void* InitData = nullptr);
	void Terminate();

	void Set(const void* constants) const;

	void BindVS(uint32_t slot = 0) const;
	void BindGS(uint32_t slot = 0) const;
	void BindPS(uint32_t slot = 0) const;
	//void BindCS(uint32_t slot = 0) const;

private:
	ID3D11Buffer* mConstantBuffer = nullptr ;
};

template <class T>
class TypedConstantBuffer : public ConstantBuffer 
{
public:
	using DataType = T;

	void Initialize() 
	{
		static_assert(sizeof(DataType) % 16 == 0, "[TypedConstantBuffer] data type must be 16 bytes aligned.");
		ConstantBuffer::Initialize(sizeof(DataType));
	}

	void Initialize(const DataType& data)
	{
		static_assert(sizeof(DataType) % 16 == 0, "[TypedConstantBuffer] data type must be 16 bytes aligned.");
		ConstantBuffer::Initialize(sizeof(DataType), &data);
	}

	void Set(const DataType& data)const 
	{
		ConstantBuffer::Set(&data);
	}
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_CONSTANTBUFFER_H
