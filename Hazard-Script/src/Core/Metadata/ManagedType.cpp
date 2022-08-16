#include "ManagedType.h"

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

	bool ManagedType::IsArray() const
	{
		return TypeEncoding == MONO_TYPE_SZARRAY || TypeEncoding == MONO_TYPE_ARRAY;
	}

	ManagedType ManagedType::FromClass(MonoClass* klass)
    {
        return ManagedType();
    }

    ManagedType ManagedType::FromType(MonoType* type)
    {
		ManagedClass klass = {};
		klass.Class = mono_type_get_class(type);

        ManagedType managedType = {};
        managedType.RawMonoType = type;
		managedType.TypeEncoding = mono_type_get_type(type);
		managedType.NativeType = GetNativeType(managedType.TypeEncoding, &klass);

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
