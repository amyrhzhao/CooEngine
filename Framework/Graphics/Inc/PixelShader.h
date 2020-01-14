#ifndef INCLUDED_COOENGINE_GRAPHICS_PIXELSHADER_H
#define INCLUDED_COOENGINE_GRAPHICS_PIXELSHADER_H

namespace Coo::Graphics {

class PixelShader 
{
public:
	void Initialize(const wchar_t* filename);
	void Terminate();

	void Bind();

private:
	ID3D11PixelShader* mPixelShader;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_PIXELSHADER_H
