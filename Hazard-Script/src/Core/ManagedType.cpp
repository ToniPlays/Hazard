#include "ManagedType.h"
#include "ScriptCache.h"

extern HazardScript::NativeType GetCustomType(const char* name);

namespace HazardScript 
{
    static NativeType GetNativeType(int typeEncoding, ManagedClass* typeClass) 
    {
        if (typeClass == nullptr) return NativeType::Undefined;

        switch (typeEncoding)
        {
        case MONO_TYPE_VOID:                return NativeType::Void;
        case MONO_TYPE_BOOLEAN:             return NativeType::Bool;
        case MONO_TYPE_CHAR:                return NativeType::UInt16;
        case MONO_TYPE_I1:                  return NativeType::Int8;
        case MONO_TYPE_I2:                  return NativeType::Int16;
        case MONO_TYPE_I4:                  return NativeType::Int32;
        case MONO_TYPE_I8:                  return NativeType::Int64;
        case MONO_TYPE_U1:                  return NativeType::UInt8;
        case MONO_TYPE_U2:                  return NativeType::UInt16;
        case MONO_TYPE_U4:                  return NativeType::UInt32;
        case MONO_TYPE_U8:                  return NativeType::UInt64;
        case MONO_TYPE_R4:                  return NativeType::Float;
        case MONO_TYPE_R8:                  return NativeType::Double;
        case MONO_TYPE_STRING:              return NativeType::String;
        case MONO_TYPE_VALUETYPE:
        {
            MonoType* classType = mono_class_get_type(typeClass->Class);
            const char* name = mono_type_get_name(classType);
            return GetCustomType(name);
        }
        }
        return NativeType::Undefined;
    }


    bool ManagedType::HasInterface(const ManagedClass* interfaceClass) const
    {
        return mono_class_is_subclass_of(TypeClass->Class, interfaceClass->Class, true);
    }
    bool ManagedType::IsSubClassOf(const ManagedClass* baseClass) const
    {
        return mono_class_is_subclass_of(TypeClass->Class, baseClass->Class, true);
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
        return TypeEncoding == MONO_TYPE_ARRAY || TypeEncoding == MONO_TYPE_SZARRAY;
    }
    bool ManagedType::IsReferenceType() const
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
        switch(TypeEncoding)
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
        if (!IsArray())
        {
            return ManagedType();
        }
        return FromClass(mono_class_get_element_class(TypeClass->Class));
    }
    ManagedType ManagedType::FromClass(ManagedClass* managedClass)
    {
        MonoType* type = mono_class_get_type(managedClass->Class);
        int typeEncoding = mono_type_get_type(type);
        return { type, typeEncoding, managedClass, GetNativeType(typeEncoding, managedClass) };

    }
    ManagedType ManagedType::FromClass(MonoClass* monoClass)
    {
        MonoType* type = mono_class_get_type(monoClass);
        int typeEncoding = mono_type_get_type(type);
        ManagedClass* managedClass = ScriptCache::GetManagedClass(monoClass);
        return { type, typeEncoding, managedClass, GetNativeType(typeEncoding, managedClass) };
    }
    ManagedType ManagedType::FromType(MonoType* monoType)
    {
        MonoClass* klass = mono_type_get_class(monoType);
        if (klass == nullptr) {
            std::cout << mono_type_get_name(monoType) << std::endl;
            return ManagedType();
        }
        return FromClass(klass);
    }
}
