#ifndef INCLUDED_COOENGINE_GRAPHICS_GEOMETRYSHADER_H
#define INCLUDED_COOENGINE_GRAPHICS_GEOMETRYSHADER_H

namespace Coo::Graphics 
{

class GeometryShader
{
public:
	void Initialize(const wchar_t* filename);
	void Terminate();

	void Bind();
	void Unbind();
private:
	ID3D11GeometryShader* mGeometryShader = nullptr;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_GEOMETRYSHADER_H
