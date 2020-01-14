#ifndef INCLUDED_COOENGINE_GRAPHICS_TEXTURE_H
#define INCLUDED_COOENGINE_GRAPHICS_TEXTURE_H

namespace Coo::Graphics {

class Texture 
{
public:
	Texture() = default;
	~Texture();
	
	void Initialize(const std::filesystem::path& fileName);
	void Terminate();

	void BindPS(uint32_t slot = 0 )const;
	void BindVS(uint32_t slot = 0 )const;

	uint32_t GetWidth() const { return mWidth; }
	uint32_t GetHeight() const { return mHeight; }

private:
	friend ID3D11ShaderResourceView* GetShaderResourceView(const Texture&);
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	uint32_t mWidth = 0;
	uint32_t mHeight = 0;	
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_TEXTURE_H
