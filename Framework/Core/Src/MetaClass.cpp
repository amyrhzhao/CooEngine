#include "Precompiled.h"
#include "MetaClass.h"
#include "MetaType.h"
#include "MetaField.h"
#include "Debug.h"
#include "MetaRegistry.h"

using namespace Coo::Core::Meta;

MetaClass::MetaClass(const char * name, size_t size, MetaType::CreateFunc create, MetaType::DestroyFunc destroy, const MetaClass * parent, const std::vector<MetaField> fields)
	: MetaType(name, MetaType::Category::Class, size, std::move(create), std::move(destroy))
	, mParent(parent)
	, mFields(std::move(fields))
{
	Register(this);
}

const MetaClass* Coo::Core::Meta::MetaClass::GetParent() const
{
	return mParent;
}

const MetaField* Coo::Core::Meta::MetaClass::FindField(const char* name) const
{
	for (auto& field : mFields)
	{
		if (strcmp(field.GetName(), name) == 0) 
		{
			return &field;
		}
	}

	// If the field is not found, try the parent
	if (mParent != nullptr) 
	{
		return mParent->FindField(name);
	}

	return nullptr;
}

const MetaField * Coo::Core::Meta::MetaClass::GetField(size_t index) const
{
	ASSERT(index < GetFieldsCount(), "Subscript out of range!");
	const size_t parentCount = GetParentFieldsCount();
	if (index < parentCount)
		return mParent->GetField(index);
	return mFields.data() + (index - parentCount);
}

const size_t Coo::Core::Meta::MetaClass::GetFieldsCount() const
{
	return mFields.size() + GetParentFieldsCount();
}

const size_t Coo::Core::Meta::MetaClass::GetParentFieldsCount() const
{
	return mParent ? mParent->GetFieldsCount() : 0u;
}
