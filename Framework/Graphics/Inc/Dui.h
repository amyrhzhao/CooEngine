#ifndef INCLUDED_COOENGINE_GRAPHICS_DUI_H
#define INCLUDED_COOENGINE_GRAPHICS_DUI_H

// Debug User Interface
namespace Coo::Graphics { class Texture; }
namespace Coo::Graphics::Dui {

enum class Theme 
{
	Classic, 
	Dark,
	Light
};

void StaticInitialize(HWND window, bool docking = false, bool multipleView = false, Theme theme = Theme::Dark);
void SetStyle(Theme theme);
void StaticTerminate();
void BeginRender();
void EndRender();

bool ImageButton(const Texture& texture, const Math::Vector2& size);

} // namespace Coo::Graphics::Dui

#endif // #ifndef INCLUDED_COOENGINE_GRAPHICS_DUI_H