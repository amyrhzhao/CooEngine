#ifndef INCLUDED_COOENGINE_CORE_METAPOINTER_H
#define INCLUDED_COOENGINE_CORE_METAPOINTER_H

#include "MetaType.h"

namespace Coo::Core::Meta 
{
	class MetaPointer : public MetaType
	{
	public:
		MetaPointer(const MetaType* pointerType);
		const MetaType* GetPointerType() const { return mPointerType; }

	private:
		const MetaType* mPointerType;
	};

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_METAPOINTER_H
