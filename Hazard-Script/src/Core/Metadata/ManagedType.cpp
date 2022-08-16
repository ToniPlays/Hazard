#include "ManagedType.h"
#include "Core/ScriptCache.h"

#include "Utility/StringUtil.h"

extern HazardScript::NativeType GetCustomType(const char* name);

namespace HazardScript 
{
	static NativeType GetNativeType(int typeEncoding, ManagedClass* typeClass)
	{
		if (typeClass == nullptr)
			return NativeType::None;
		switch (typeEncoding)
		{
		case MONO_TYPE_VOID:                return NativeType::Void;
		case MONO_TYPE_BOOLEAN:             return NativeType::Bool;
		case MONO_TYPE_CHAR:                return NativeType::UInt16;
		case MONO_TYPE_I1:                  return NativeType::Int8;
		case MONO_TYPE_I2:                  return NativeType::Int16;
		case MONO_TYPE_I4:                  return NativeType::Int32;
		case MONO_TYPE_I8:                  return NativeType::Int64;
		case MONO_TYPE_I:					return NativeType::Int32;
		case MONO_TYPE_U1:                  return NativeType::UInt8;
		case MONO_TYPE_U2:                  return NativeType::UInt16;
		case MONO_TYPE_U4:                  return NativeType::UInt32;
		case MONO_TYPE_U8:                  return NativeType::UInt64;
		case MONO_TYPE_U:					return NativeType::UInt32;
		case MONO_TYPE_R4:                  return NativeType::Float;
		case MONO_TYPE_R8:                  return NativeType::Double;
		case MONO_TYPE_STRING:              return NativeType::String;
		case MONO_TYPE_VALUETYPE:
		{
			MonoType* classType = mono_class_get_type(typeClass->Class);
			const char* name = mono_type_get_name(classType);
			return GetCustomType(name);
		}
		case MONO_TYPE_CLASS:
		{
			return NativeType::Reference;
		}
		}
		return NativeType::None;
	}

	bool ManagedType::HasInterface(const ManagedClass* interfaceClass) const
	{
		return mono_class_is_subclass_of(TypeClass->Class, interfaceClass->Class, true);
	}

	bool ManagedType::IsSubClassOf(const ManagedClass* subclass) const
	{
		return mono_class_is_subclass_of(TypeClass->Class, subclass->Class, false);
	}

	bool ManagedType::CanAssignTo(const ManagedClass* managedClass) const
	{
		return mono_class_is_assignable_from(TypeClass->Class, managedClass->Class);
	}

	bool ManagedType::IsVoid() const
	{
		return TypeEncoding == MONO_TYPE_VOID;
	}

	bool ManagedType::IsArray() const
	{
		return TypeEncoding == MONO_TYPE_SZARRAY || TypeEncoding == MONO_TYPE_ARRAY;
	}

	bool ManagedType::IsReference() const
	{
		return mono_type_is_reference(RawMonoType) || mono_type_is_byref(RawMonoType);
	}

	bool ManagedType::IsValueType() const
	{
		return TypeEncoding == MONO_TYPE_VALUETYPE;
	}

	bool ManagedType::IsGeneric() const
	{
		return TypeEncoding == MONO_TYPE_GENERICINST;
	}

	bool ManagedType::IsPrimitive() const
	{
		switch (TypeEncoding)
		{
		case MONO_TYPE_VOID:
		case MONO_TYPE_STRING:
		case MONO_TYPE_CLASS:
		case MONO_TYPE_SZARRAY:
		case MONO_TYPE_ARRAY:
			return false;
		}
		return true;
	}

	ManagedType ManagedType::GetElementType() const
	{
		if (!IsArray()) return ManagedType();
		return FromClass(mono_class_get_element_class(TypeClass->Class));
	}

	std::string ManagedType::GetTypeName() const
	{
		return mono_type_get_name(RawMonoType);
	}

	ManagedType ManagedType::FromClass(MonoClass* klass)
    {
        return ManagedType();
    }

    ManagedType ManagedType::FromType(MonoType* type)
    {
		MonoClass* typeClass = mono_type_get_class(type);
			std::string name = mono_type_get_name(type);
		if (typeClass == nullptr) 
		{
			std::string nameSpace = name.substr(0, StringUtil::OffsetOf(name, '.'));
			name = name.substr(StringUtil::OffsetOf(name, '.') + 1);

			typeClass = mono_class_from_name(mono_get_corlib(), nameSpace.c_str(), name.c_str());
		}

        ManagedType managedType = {};
        managedType.RawMonoType = type;
		managedType.TypeClass = ScriptCache::GetClass(typeClass);
		managedType.TypeEncoding = mono_type_get_type(type);
		managedType.NativeType = GetNativeType(managedType.TypeEncoding, managedType.TypeClass);

		return managedType;
    }
	ManagedMethod ManagedMethod::FromMethod(MonoMethod* method)
	{
		MonoMethodSignature* sig = mono_method_signature(method);
		MonoType* type = mono_signature_get_return_type(sig);

		ManagedMethod managedMethod = {};
		managedMethod.Method = method;
		managedMethod.ReturnType = ManagedType::FromType(type);

		return managedMethod;
	}
}
