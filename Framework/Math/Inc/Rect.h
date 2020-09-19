#pragma once

#include "Vector2.h"

namespace Coo::Math {

	struct Rect
	{
#pragma warning(push)
#pragma warning(disable: 4201)
		union
		{
			struct { float left, top; };
			Coo::Math::Vector2 min;
		};
		union
		{
			struct { float right, bottom; };
			Coo::Math::Vector2 max;
		};
#pragma warning(pop)

		constexpr Rect() : Rect{ 0.0f, 0.0f, 1.0f, 1.0f } {}
		constexpr Rect(float left, float top, float right, float bottom) : left{ left }, top{ top }, right{ right }, bottom{ bottom } {}
	};
}