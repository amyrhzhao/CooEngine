#ifndef INCLUDED_COOENGINE_GRAPHICS_FONT_H
#define INCLUDED_COOENGINE_GRAPHICS_FONT_H

interface IFW1Factory;
interface IFW1FontWrapper;

namespace Coo::Graphics 
{

class Font 
{
public:	
	void Initialize();
	void Terminate();

	void Draw(const wchar_t* str, float x, float y, float size, uint32_t color);

private:
	IFW1Factory* mFW1Factory = nullptr;
	IFW1FontWrapper* mFontWrapper = nullptr;
};

} // namespace Coo::Graphics

#endif // !INCLUDED_COOENGINE_GRAPHICS_FONT_H
