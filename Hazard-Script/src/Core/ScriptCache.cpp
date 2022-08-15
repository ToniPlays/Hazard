
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
	}

	void ScriptCache::CacheClass(const std::string& name, MonoClass* monoClass)
	{
		if (name == ".<Module>") return;
		//std::cout << "Caching " << name << std::endl;

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
		if (s_Cache->Classes.find(classID) == s_Cache->Classes.end()) return nullptr;
		return &s_Cache->Classes[classID];
	}
}



