#pragma once

#include "Mono/Core/Mono.h"

namespace HazardScript 
{
	struct ManagedClass 
	{
		MonoClass* Class;
		std::string FullName;
		uint32_t ID = 0;
		uint32_t Size;
	};

	struct ManagedType 
	{
		MonoType* RawMonoType;
		NativeType NativeType;
		ManagedClass* TypeClass;

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

		std::string GetTypeName() const;
		static ManagedType FromClass(MonoClass* klass);
		static ManagedType FromType(MonoType* type);
	};


	struct ManagedMethod 
	{
		MonoMethod* Method = nullptr;
		ManagedType ReturnType;

		static ManagedMethod FromMethod(MonoMethod* method);

	};
}