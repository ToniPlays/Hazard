#pragma once

#include "Mono/Core/Mono.h"

namespace HazardScript 
{
	struct ManagedClass 
	{
		MonoClass* Class;
	};

	struct ManagedType 
	{
		MonoType* RawMonoType;
		NativeType NativeType;

		int TypeEncoding = 0;

		bool IsArray() const;


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