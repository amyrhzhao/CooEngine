#ifndef INCLUDED_COOENGINE_GRAPHICS_SIMPLEDRAW_H
#define INCLUDED_COOENGINE_GRAPHICS_SIMPLEDRAW_H

#include "Colors.h"
#include "Camera.h"

namespace Coo::Graphics::SimpleDraw {

void StaticInitialize(uint32_t maxVertexCount = 100000);
void StaticTerminate();

// Debug Draw
void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
void AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t hCount = 8, uint32_t vCount = 8);
void AddAABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, const Color& color);
void AddAABB(const Math::Vector3& pos, float radius, const Color& color);
void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
void AddScreenCircle(const Math::Vector2& center, float radius, const Color& color);
void AddScreenRect(float minX, float maxX, float minY, float maxY, const Color& color);
void AddScreenText(const char* str, float x, float y, float size, const Color& color);

void Render(const Camera& camera);

} // namespace Coo::Graphics::SimpleDraw

#endif // !INCLUDED_COOENGINE_GRAPHICS_SIMPLEDRAW_H
