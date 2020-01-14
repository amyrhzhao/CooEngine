#ifndef INCLUDED_COOENGINE_GRAPHICS_VERTEXSHADER_H
#define INCLUDED_COOENGINE_GRAPHICS_VERTEXSHADER_H

namespace Coo::Graphics {

	class VertexShader
	{
	public:
		void Initialize(const wchar_t* filename, uint32_t vertexFormat);
		void Terminate();

		void Bind();

	private:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;
	};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_VERTEXSHADER_H
