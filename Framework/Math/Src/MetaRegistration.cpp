#include "Precompiled.h"
#include "MetaRegistration.h"

namespace Coo::Core::Meta 
{
	using namespace Coo::Math;
	template <>
	void Deserialize<Vector2>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vec = (Vector2*)(instance);
		vec->x = object.FindMember("x")->value.GetFloat();
		vec->y = object.FindMember("y")->value.GetFloat();
	}

	template <>
	void Deserialize<Vector3>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vec = (Vector3*)(instance);
		vec->x = object.FindMember("x")->value.GetFloat();
		vec->y = object.FindMember("y")->value.GetFloat();
		vec->z = object.FindMember("z")->value.GetFloat();
	}

	template <>
	void Deserialize<Vector4>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vec = (Vector4*)(instance);
		vec->x = object.FindMember("x")->value.GetFloat();
		vec->y = object.FindMember("y")->value.GetFloat();
		vec->z = object.FindMember("z")->value.GetFloat();
		vec->w = object.FindMember("w")->value.GetFloat();
	}
	
	template <>
	void Deserialize<Quaternion>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vec = (Quaternion*)(instance);
		vec->x = object.FindMember("x")->value.GetFloat();
		vec->y = object.FindMember("y")->value.GetFloat();
		vec->z = object.FindMember("z")->value.GetFloat();
		vec->w = object.FindMember("w")->value.GetFloat();
	}
}

// Primitive Type Definition
//META_TYPE_DEFINE(Coo::Math::Matrix3, Matrix3)
//META_TYPE_DEFINE(Coo::Math::Matrix4, Matrix4)
META_TYPE_DEFINE(Coo::Math::Quaternion, Quaternion)
META_TYPE_DEFINE(Coo::Math::Vector2, Vector2)
META_TYPE_DEFINE(Coo::Math::Vector3, Vector3)
META_TYPE_DEFINE(Coo::Math::Vector4, Vector4)

void Coo::Math::StaticRegister()
{}