#pragma once

#include "Mono/Core/Mono.h"
#include "Metadata/ManagedType.h"
#include "Metadata/FieldMetadata.h"
#include "Metadata/ScriptMetadata.h"

namespace HazardScript 
{
	struct ScriptCacheData
	{
		std::unordered_map<uint32_t, ManagedClass> Classes;
		std::unordered_map<uint32_t, ScriptMetadata> ScriptMetadata;
		std::unordered_map<uint32_t, FieldMetadata> ScriptFields;
	};

	class ScriptCache 
	{
	public:
		static void Init();

		static ManagedClass* CacheClass(const std::string& className, MonoClass* klass);
		static ManagedClass* GetClass(MonoClass* monoClass);

		static ScriptMetadata* CacheOrGetScriptMetadata(ManagedClass* klass);
		static FieldMetadata* CacheOrGetFieldMetadata(MonoClassField* field);

		static ScriptCacheData* GetCache() { return s_Cache; }

	private:
		static ManagedClass* GetManagedClassByName(const std::string& name);

		static void CacheCoreLibClasses();

		inline static ScriptCacheData* s_Cache;
	};
}