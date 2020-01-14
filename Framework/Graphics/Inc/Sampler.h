#ifndef INCLUDED_COOENGINE_GRAPHICS_SAMPLER_H
#define INCLUDED_COOENGINE_GRAPHICS_SAMPLER_H

namespace Coo::Graphics {

class Sampler
{
public:
	enum class Filter { Point, Linear, Anisotropic }; // what do i do with uv and color
	enum class AddressMode { Border, Clamp, Mirror, Wrap}; // what do i do with uv

	Sampler() = default;
	~Sampler();

	void Initialize(Filter filter, AddressMode addressMode);
	void Terminate();

	void BindPS(uint32_t slot = 0) const;

private:
	ID3D11SamplerState* mSampler{ nullptr };
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_SAMPLER_H
