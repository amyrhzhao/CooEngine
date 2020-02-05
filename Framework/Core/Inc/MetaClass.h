#ifndef INCLUDED_COOENGINE_CORE_METACLASS_H
#define INCLUDED_COOENGINE_CORE_METACLASS_H

#include "MetaType.h"

namespace Coo::Core::Meta
{
	class MetaField;

	class MetaClass : public MetaType 
	{
	public:
		MetaClass(
			const char* name,
			size_t size,
			MetaType::CreateFunc create, 
			MetaType::DestroyFunc destroy, 
			const MetaClass* parent,
			const std::vector<MetaField> fields
		);

		const MetaClass* GetParent() const;
		const MetaField* FindField(const char* name) const;
		const MetaField* GetField(size_t index) const;
		const size_t GetFieldsCount() const;

	private:
		const size_t GetParentFieldsCount() const;

		const MetaClass* mParent;
		const std::vector<MetaField> mFields;
	};

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_METACLASS_H
