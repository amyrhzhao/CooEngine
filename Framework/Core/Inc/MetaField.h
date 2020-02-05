#ifndef INCLUDED_COOENGINE_CORE_METAFIELD_H
#define INCLUDED_COOENGINE_CORE_METAFIELD_H

namespace Coo::Core::Meta 
{
	class MetaType;

	class MetaField 
	{
	public:
		MetaField(const char* name, const MetaType* type, size_t offset);
	
		const MetaType* GetMetaType() const { return mType; }
		const char* GetName() const { return mName.c_str(); }
		const size_t GetOffset() const { return mOffset; }

	private:
		const MetaType* mType;
		const std::string mName;
		const size_t mOffset;
	};

} // namespace Coo::Core::Meta

#endif // !INCLUDED_COOENGINE_CORE_METAFIELD_H
