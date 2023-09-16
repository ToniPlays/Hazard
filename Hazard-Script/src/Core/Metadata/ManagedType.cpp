
#include "ManagedType.h"
#include "Core/ScriptCache.h"

#include "Utility/StringUtil.h"

extern HazardScript::NativeType GetCustomType(const char* name);

namespace HazardScript
{
	static NativeType GetNativeType(int typeEncoding, ManagedClass* typeClass)
	{
#ifdef SOmeRADNOMGSHIT
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
		case MONO_TYPE_SZARRAY:             return NativeType::Reference;
		case MONO_TYPE_ARRAY:				return NativeType::Reference;
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
#endif
		return NativeType::None;
	}

	bool ManagedType::HasInterface(const ManagedClass* interfaceClass) const
	{
		return false;
		//return mono_class_is_subclass_of(TypeClass->Class, interfaceClass->Class, true);
	}

	bool ManagedType::IsSubClassOf(const ManagedClass* baseClass) const
	{
		return false;
        //return mono_class_is_subclass_of(TypeClass->Class, baseClass->Class, false);
	}

	bool ManagedType::CanAssignTo(const ManagedClass* managedClass) const
	{
		return false;
        //return mono_class_is_assignable_from(TypeClass->Class, managedClass->Class);
	}

	bool ManagedType::IsVoid() const
	{
		return false;
		//return TypeEncoding == MONO_TYPE_VOID;
	}

	bool ManagedType::IsArray() const
	{
		return false;
		//return TypeEncoding == MONO_TYPE_SZARRAY || TypeEncoding == MONO_TYPE_ARRAY;
	}

	bool ManagedType::IsReference() const
	{
		return false;
		//return mono_type_is_reference(RawMonoType) || mono_type_is_byref(RawMonoType);
	}

	bool ManagedType::IsValueType() const
	{
		return false;
		//return TypeEncoding == MONO_TYPE_VALUETYPE;
	}

	bool ManagedType::IsGeneric() const
	{
		return false;
		//return TypeEncoding == MONO_TYPE_GENERICINST;
	}

	bool ManagedType::IsPrimitive() const
	{
		/*
		switch (TypeEncoding)
		{
		case MONO_TYPE_VOID:
		case MONO_TYPE_STRING:
		case MONO_TYPE_CLASS:
		case MONO_TYPE_SZARRAY:
		case MONO_TYPE_ARRAY:
			return false;
		}
		*/
		return true;
	}

	bool ManagedType::IsFixedSize() const
	{
		return IsArray() ? false : false;
	}

	ManagedType ManagedType::GetElementType() const
	{
		if (!IsArray()) return ManagedType();
		return ManagedType();
		//return FromClass(mono_class_get_element_class(TypeClass->Class));
	}
	/*
	ManagedType ManagedType::FromClass(MonoClass* klass)
	{
		MonoType* type = mono_class_get_type(klass);
		int typeEncoding = mono_type_get_type(type);
		ManagedClass* managedClass = ScriptCache::GetClass(klass);

		std::string name = mono_type_get_name(type);
		name = name.substr(name.find_last_of('.') + 1);

		return { type, GetNativeType(typeEncoding, managedClass), managedClass, managedClass->FullName, name, typeEncoding};
	}
	*/
	/*
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
		managedType.FullName = managedType.TypeClass->FullName;
		managedType.DisplayName = name.substr(name.find_last_of('.') + 1);

		return managedType;
	}
	ManagedMethod ManagedMethod::FromMethod(MonoMethod* method)
	{
		MonoMethodSignature* sig = mono_method_signature(method);
		MonoType* type = mono_signature_get_return_type(sig);

		ManagedMethod managedMethod = {};
		managedMethod.Method = method;
		if (!mono_type_is_generic_parameter(type))
			managedMethod.ReturnType = ManagedType::FromType(type);

		return managedMethod;
	}
	*/
}
