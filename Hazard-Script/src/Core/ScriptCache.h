#pragma once

#include "Mono/Core/Mono.h"
#include "Metadata/ManagedType.h"

namespace HazardScript 
{
	class ScriptCache 
	{
	public:
		static void Init();

		static void CacheClass(const std::string& className, MonoClass* klass);
		static ManagedClass* GetClass(MonoClass* monoClass);

	private:

		static ManagedClass* GetManagedClassByName(const std::string& name);

		static void CacheCoreLibClasses();
	};
}