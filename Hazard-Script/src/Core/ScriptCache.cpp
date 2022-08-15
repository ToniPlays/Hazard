
#include "ScriptCache.h"
#include <unordered_map>

#include "Hash.h"

namespace HazardScript 
{
	struct Cache {
		std::unordered_map<uint32_t, ManagedClass> Classes;
		std::unordered_map<uint32_t, ManagedField> Fields;
	};

	static Cache* s_Cache = nullptr;

	void ScriptCache::Init()
	{
		s_Cache = new Cache();
		CacheCoreLibClasses();
	}

	void ScriptCache::CacheClass(const std::string& name, MonoClass* monoClass)
	{
		if (name == ".<Module>") return;

		MonoType* type = mono_class_get_type(monoClass);

		ManagedClass managedClass = {};
		managedClass.FullName = name;
		managedClass.ID = Hash::GenerateFNVHash(managedClass.FullName);
		int alignment = 0;
		managedClass.Size = mono_type_size(type, &alignment);
		managedClass.Class = monoClass;

		s_Cache->Classes[managedClass.ID] = managedClass;
	}
	ManagedClass* ScriptCache::GetManagedClass(MonoClass* monoClass)
	{
		if (monoClass == nullptr) return nullptr;
		if (s_Cache == nullptr) return nullptr;

		return GetManagedClassByName(Mono::ResolveClassName(monoClass));
	}
	ManagedClass* ScriptCache::GetManagedClassByName(const std::string& name)
	{
		if (s_Cache == nullptr) return nullptr;
		uint32_t classID = Hash::GenerateFNVHash(name);
		if (s_Cache->Classes.find(classID) == s_Cache->Classes.end()) 
			return nullptr;
		return &s_Cache->Classes[classID];
	}
	void ScriptCache::CacheCoreLibClasses()
	{
		std::cout << "Caching core lib classes" << std::endl;
		#define CACHE_CORELIB_CLASS(name) CacheClass("System." ##name, mono_class_from_name(mono_get_corlib(), "System", name))
		CACHE_CORELIB_CLASS("Object");
		CACHE_CORELIB_CLASS("ValueType");
		CACHE_CORELIB_CLASS("Boolean");
		CACHE_CORELIB_CLASS("SByte");
		CACHE_CORELIB_CLASS("Int16");
		CACHE_CORELIB_CLASS("Int32");
		CACHE_CORELIB_CLASS("Int64");
		CACHE_CORELIB_CLASS("Byte");
		CACHE_CORELIB_CLASS("UInt16");
		CACHE_CORELIB_CLASS("UInt32");
		CACHE_CORELIB_CLASS("UInt64");
		CACHE_CORELIB_CLASS("IntPtr");
		CACHE_CORELIB_CLASS("Single");
		CACHE_CORELIB_CLASS("Double");
		CACHE_CORELIB_CLASS("Char");
		CACHE_CORELIB_CLASS("String");
	}
}



