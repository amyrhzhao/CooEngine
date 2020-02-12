#include "Precompiled.h"
#include "TransformComponent.h"

using namespace Coo;

META_DERIVED_BEGIN(Coo::TransformComponent, Coo::Component) 
	META_FIELD_BEGIN
	META_FIELD(position, "Position")
	META_FIELD(rotation, "Rotation")
	META_FIELD_END
META_CLASS_END