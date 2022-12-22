#pragma once

#include "Mono/Core/Mono.h"

namespace HazardScript 
{
	enum MonoFlags : uint32_t
	{
		MonoFlags_Private = BIT(0),
		MonoFlags_Protected = BIT(1),
		MonoFlags_Public = BIT(2),
		MonoFlags_Internal = BIT(3),
		MonoFlags_Static = BIT(4),
		MonoFlags_Abstract = BIT(5),
		MonoFlags_Virtual = BIT(6)
	};

	enum class NativeType {
		None,
		Void,
		Bool,
		Float, Float2, Float3, Float4,
		Double,
		Int8, Int16, Int32, Int64,
		UInt8, UInt16, UInt32, UInt64,
		String,
		Value,
		Reference
	};

	struct ManagedClass 
	{
#ifdef HZR_INCLUDE_MONO
		MonoClass* Class;
#endif
		std::string FullName;
		uint32_t ID = 0;
		uint32_t Size;
	};

	struct ManagedType 
	{
#ifdef HZR_INCLUDE_MONO
		MonoType* RawMonoType = nullptr;
#else
        void* RawMonoType = nullptr;
#endif
		NativeType NativeType;
		ManagedClass* TypeClass = nullptr;

		std::string FullName;
		std::string DisplayName;

		int TypeEncoding = 0;

		bool HasInterface(const ManagedClass* interfaceClass) const;
		bool IsSubClassOf(const ManagedClass* baseClass) const;
		bool CanAssignTo(const ManagedClass* managedClass) const;
		bool IsVoid() const;
		bool IsArray() const;
		bool IsReference() const;
		bool IsValueType() const;
		bool IsGeneric() const;
		bool IsPrimitive() const;
		bool IsFixedSize() const;
		ManagedType GetElementType() const;

		bool IsValid() const { return TypeEncoding != 0 && RawMonoType != nullptr && NativeType != NativeType::None; }

#ifdef HZR_INCLUDE_MONO
		static ManagedType FromClass(MonoClass* klass);
		static ManagedType FromType(MonoType* type);
#endif
	};


	struct ManagedMethod 
	{
        ManagedType ReturnType;
#ifdef HZR_INCLUDE_MONO
		MonoMethod* Method = nullptr;
        static ManagedMethod FromMethod(MonoMethod* method);
#endif
	
	};
}

