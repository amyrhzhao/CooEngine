#ifndef INCLUDED_COOENGINE_GRAPHICS_D3DUTIL_H
#define INCLUDED_COOENGINE_GRAPHICS_D3DUTIL_H

namespace Coo::Graphics
{
class Texture;

ID3D11ShaderResourceView* GetShaderResourceView(const Texture& texture);

ID3D11Device* GetDevice();
ID3D11DeviceContext* GetContext();

} // namespce Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_D3DUTIL_H
