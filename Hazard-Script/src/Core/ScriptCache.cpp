#include "ScriptCache.h"
#ifdef HZR_INCLUDE_MONO

#include "UtilityCore.h"
#include "Mono/Core/Mono.h"
#include "Hash.h"

#include "Metadata/ScriptMetadata.h"

namespace HazardScript 
{

#define CACHE_CORELIB_CLASS(name) CacheClass("System." ##name, mono_class_from_name(mono_get_corlib(), "System", name))

	void ScriptCache::Init()
	{
		s_Cache = hnew ScriptCacheData();
		CacheCoreLibClasses();
	}
	ManagedClass* ScriptCache::CacheClass(const std::string& className, MonoClass* klass)
	{
		MonoType* classType = mono_class_get_type(klass);

		ManagedClass managedClass = {};
		managedClass.Class = klass;
		managedClass.FullName = className;
		managedClass.ID = Hash::GenerateFNVHash(className);

		int alignment;
		managedClass.Size = mono_type_size(classType, &alignment);

		s_Cache->Classes[managedClass.ID] = managedClass;
		return &s_Cache->Classes[managedClass.ID];
	}
	ManagedClass* ScriptCache::GetClass(MonoClass* monoClass)
	{
		if (monoClass == nullptr) return nullptr;

		return GetManagedClassByName(Mono::ResolveClassName(monoClass));
	}
	Ref<ScriptMetadata> ScriptCache::CacheOrGetScriptMetadata(ManagedClass* klass)
	{
		uint32_t hash = Hash::GenerateFNVHash(klass->FullName);
		if (s_Cache->ScriptMetadata.find(hash) != s_Cache->ScriptMetadata.end())
			return s_Cache->ScriptMetadata[hash];

		s_Cache->ScriptMetadata[hash] = Ref<ScriptMetadata>::Create(klass);
		return s_Cache->ScriptMetadata[hash];

	}
	Ref<FieldMetadata> ScriptCache::CacheOrGetFieldMetadata(MonoClassField* field)
	{
		const char* name = mono_field_full_name(field);
		uint32_t hash = Hash::GenerateFNVHash(name);

		if (s_Cache->ScriptFields.find(hash) != s_Cache->ScriptFields.end())
			return s_Cache->ScriptFields[hash];

		Ref<FieldMetadata> metadata = Ref<FieldMetadata>::Create(field);
		s_Cache->ScriptFields[hash] = metadata;
		return s_Cache->ScriptFields[hash];
	}
	ManagedClass* ScriptCache::GetManagedClassByName(const std::string& name)
	{
		uint32_t classID = Hash::GenerateFNVHash(name);
		if (s_Cache->Classes.find(classID) == s_Cache->Classes.end())
			return nullptr;

		return &s_Cache->Classes[classID];
	}
	void ScriptCache::CacheCoreLibClasses()
	{
		CACHE_CORELIB_CLASS("Object");
		CACHE_CORELIB_CLASS("ValueType");
		CACHE_CORELIB_CLASS("Boolean");
		CACHE_CORELIB_CLASS("SByte");
		CACHE_CORELIB_CLASS("Int16");
		CACHE_CORELIB_CLASS("Int32");
		CACHE_CORELIB_CLASS("Int64");
		CACHE_CORELIB_CLASS("UInt16");
		CACHE_CORELIB_CLASS("UInt32");
		CACHE_CORELIB_CLASS("UInt64");
		CACHE_CORELIB_CLASS("Byte");
		CACHE_CORELIB_CLASS("UInt16");
		CACHE_CORELIB_CLASS("UInt32");
		CACHE_CORELIB_CLASS("UInt64");
		CACHE_CORELIB_CLASS("Single");
		CACHE_CORELIB_CLASS("Double");
		CACHE_CORELIB_CLASS("Char");
		CACHE_CORELIB_CLASS("String");
		CACHE_CORELIB_CLASS("Void");

		CacheClass("System.Diagnostics.StackTrace", mono_class_from_name(mono_get_corlib(), "System.Diagnostics", "StackTrace"));
	}
}
#endif
