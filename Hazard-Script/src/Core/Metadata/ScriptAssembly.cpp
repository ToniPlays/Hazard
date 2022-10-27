
#include "ScriptAssembly.h"
#include "Buffer.h"
#include "File.h"

#include "Mono/Core/Mono.h"
#include "Core/ScriptCache.h"

namespace HazardScript 
{
	ScriptAssembly::~ScriptAssembly()
	{
		Release();
		__debugbreak();
	}
	bool ScriptAssembly::LoadFromSource(bool registerScripts, bool withRefecenced)
	{
		Release();
		if (m_Path.empty()) return false;

		CachedBuffer data = File::ReadBinaryFile(m_Path);
		
		MonoImageOpenStatus status;
		MonoImage* image = Mono::OpenImage((char*)data.GetData(), data.GetSize(), status);
		HZR_ASSERT(image, "Image fail");
		m_Assembly = Mono::AssemblyFromImage(image, m_Path, status);
		mono_image_close(image);

		if (withRefecenced) 
			LoadReferencedAssemblies();

		if(registerScripts)
			LoadScripts();

		return true;
	}
	MonoImage* ScriptAssembly::GetImage() const
	{
		return mono_assembly_get_image(m_Assembly);
	}
	void ScriptAssembly::LoadReferencedAssemblies()
	{
		const MonoTableInfo* t = mono_image_get_table_info(GetImage(), MONO_TABLE_ASSEMBLYREF);
		int rows = mono_table_info_get_rows(t);
		
		m_ReferencedAssemblies.clear();
		m_ReferencedAssemblies.reserve(rows);
		for (int i = 0; i < rows; i++) 
		{
			uint32_t cols[MONO_ASSEMBLYREF_SIZE];
			mono_metadata_decode_row(t, i, cols, MONO_ASSEMBLYREF_SIZE);

			auto& data = m_ReferencedAssemblies.emplace_back();
			data.Name = mono_metadata_string_heap(GetImage(), cols[MONO_ASSEMBLYREF_NAME]);
		}
	}
	void ScriptAssembly::LoadScripts()
	{
		m_Scripts.clear();

		const MonoTableInfo* tableInfo = mono_image_get_table_info(GetImage(), MONO_TABLE_TYPEDEF);
		uint32_t rows = mono_table_info_get_rows(tableInfo);

		std::vector<ManagedClass*> monoClasses;
		monoClasses.reserve(rows);

		for (uint32_t i = 0; i < rows; i++) 
		{
			MonoClass* monoClass = nullptr;
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap(GetImage(), cols[MONO_TYPEDEF_NAME]);
			const char* nameSpace = mono_metadata_string_heap(GetImage(), cols[MONO_TYPEDEF_NAMESPACE]);
			if (strcmp(name, "<Module>") == 0) continue;

			std::string fullName = nameSpace;
			if (!fullName.empty()) fullName += ".";
			fullName += name;

			monoClass = Mono::GetMonoClass(GetImage(), nameSpace, name);
			ManagedClass* managedClass = ScriptCache::CacheClass(fullName, monoClass);
			monoClasses.push_back(managedClass);
		}

		for (ManagedClass* klass : monoClasses) 
		{
			Ref<ScriptMetadata> script = ScriptCache::CacheOrGetScriptMetadata(klass);
			script->UpdateMetadata();
			m_Scripts[script->GetName()] = script;

			//std::cout << script->GetName() << std::endl;
		}
	}
	void ScriptAssembly::Release()
	{
		if (m_Assembly)
			mono_assembly_close(m_Assembly);
	}
}