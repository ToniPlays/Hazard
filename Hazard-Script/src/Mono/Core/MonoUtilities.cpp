
#include "MonoUtilities.h"

namespace HazardScript 
{
	void MonoArrayUtils::SetValueInternal(MonoArray* arrayObject, size_t index, const ValueWrapper& value)
	{
		uintptr_t length = mono_array_length(arrayObject);

		if (index >= length) __debugbreak();

		MonoClass* arrayClass = mono_object_get_class((MonoObject*)arrayObject);
		MonoClass* elementClass = mono_class_get_element_class(arrayClass);
		int32_t size = mono_array_element_size(arrayClass);
		ManagedType type = ManagedType::FromClass(elementClass);

		if (type.IsReference()) 
		{
			//MonoObject* boxed = nullptr;
			//mono_array_setref(arrayObject, index, boxed);
		}
		else
		{
			char* dst = mono_array_addr_with_size(arrayObject, size, index);
			memcpy(dst, value.GetRawData(), size);
		}
	}
	void MonoArrayUtils::SetValueInternal(MonoArray* arrayObject, size_t index, MonoObject* value) 
	{
		uintptr_t length = mono_array_length(arrayObject);

		if (index >= length) {
			__debugbreak();
		}

		MonoClass* arrayClass = mono_object_get_class((MonoObject*)arrayObject);
		MonoClass* elementClass = mono_class_get_element_class(arrayClass);
		int32_t size = mono_array_element_size(arrayClass);
		ManagedType type = ManagedType::FromClass(elementClass);

		if (type.IsReference())
			mono_array_setref(arrayObject, index, value);
		else
			mono_array_set(arrayObject, MonoObject*, index, value);
	}
}