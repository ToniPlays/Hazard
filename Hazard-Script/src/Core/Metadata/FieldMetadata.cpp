
#include "FieldMetadata.h"
#include "Core/AttributeBuilder.h"
#include "Core/FieldValueStorage.h"

namespace HazardScript
{
	/*
	FieldMetadata::FieldMetadata(MonoClassField* field)
	{
		m_Field = field;
		m_Name = mono_field_get_name(field);
		m_Type = ManagedType::FromType(mono_field_get_type(field));

		LoadAttributes();
	}
	*/
	uint32_t FieldMetadata::GetElementCount(const Coral::ManagedObject& handle)
	{
		/*
		if (!m_Type.IsArray()) return 1;
		MonoObject* target = mono_gchandle_get_target(handle);
		return m_InstanceData[handle].As<ArrayFieldValueStorage>()->GetLength(mono_field_get_value_object(Mono::GetDomain(), GetMonoField(), target));
		*/
		return 0;
	}
	void FieldMetadata::SetArraySize(const Coral::ManagedObject& handle, uint32_t elements)
	{
		/*
		HZR_ASSERT(m_Type.IsArray(), "Attempted to set array size of non array type");
		MonoObject* target = mono_gchandle_get_target(handle);
		m_InstanceData[handle].As<ArrayFieldValueStorage>()->Resize(target, elements);
		*/
	}
	void FieldMetadata::LoadAttributes()
	{
		
	}
}
