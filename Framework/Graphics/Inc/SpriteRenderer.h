#ifndef INCLUDED_COOENGINE_GRAPHICS_SPRITERENDERER_H
#define INCLUDED_COOENGINE_GRAPHICS_SPRITERENDERER_H

namespace DirectX { class SpriteBatch; class CommonStates; }

namespace Coo::Graphics
{
class Texture;

class SpriteRenderer 
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SpriteRenderer* Get();

	SpriteRenderer() = default;
	~SpriteRenderer();

	void Initialize();
	void Terminate();

	void BeginRender();
	void EndRender();

	void Draw(const Texture& texture, const Math::Vector2& pos);

private:
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch = nullptr;
	std::unique_ptr<DirectX::CommonStates> mCommonStates = nullptr;
};
} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_SPRITERENDERER_H
