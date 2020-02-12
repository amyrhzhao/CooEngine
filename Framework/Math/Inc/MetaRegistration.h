#ifndef INCLUDED_COOENGINE_MATH_METAREGISTRATION_H
#define INCLUDED_COOENGINE_MATH_METAREGISTRATION_H

#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

// Primitive Type Declarations
META_TYPE_DECLARE(Coo::Math::Matrix3)
META_TYPE_DECLARE(Coo::Math::Matrix4)
META_TYPE_DECLARE(Coo::Math::Quaternion)
META_TYPE_DECLARE(Coo::Math::Vector2)
META_TYPE_DECLARE(Coo::Math::Vector3)
META_TYPE_DECLARE(Coo::Math::Vector4)

namespace Coo::Math
{
	void StaticRegister();
}
#endif //!INCLUDED_COOENGINE_MATH_METAREGISTRATION_H