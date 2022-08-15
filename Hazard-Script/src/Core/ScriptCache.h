#pragma once

#include "Mono/Core/Mono.h"
#include "ManagedType.h"

namespace HazardScript 
{
	class ScriptCache {
	public:
		static void Init();

		static void CacheClass(const std::string& name, MonoClass* monoClass);

		static ManagedClass* GetManagedClass(MonoClass* monoClass);
	private:
		static ManagedClass* GetManagedClassByName(const std::string& name);
	};
}