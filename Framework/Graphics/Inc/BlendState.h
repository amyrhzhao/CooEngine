#ifndef INCLUDED_COOENGINE_GRAPHICS_BLENDSTATE_H
#define INCLUDED_COOENGINE_GRAPHICS_BLENDSTATE_H

namespace Coo::Graphics
{
class BlendState 
{
public:
	static void ClearState();

public:
	enum class Mode
	{
		Opaque,
		AlphaBlend,
		AlphaPremultiplied,
		Additive
	};

	BlendState() = default;
	~BlendState();

	void Initialize(Mode mode);
	void Terminate();

	void Set();

private:
	ID3D11BlendState* mBlendState = nullptr;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_BLENDSTATE_H
