
#include "ScriptAssembly.h"
#include "Buffer.h"
#include "File.h"
#include "Mono/Core/Mono.h"

#include "Core/ScriptCache.h"
#include <sstream>

namespace HazardScript
{
	bool ScriptAssembly::LoadFromSource(bool registerScripts)
	{
		if (m_Path.empty()) return false;

		Buffer data = File::ReadBinaryFile(m_Path);

		MonoImageOpenStatus status;
		MonoImage* image = Mono::OpenImage((char*)data.Data, data.Size, status);
		m_Assembly = Mono::AssemblyFromImage(image, m_Path, status);

		mono_image_close(image);

		const MonoTableInfo* tableInfo = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		uint32_t rows = mono_table_info_get_rows(tableInfo);

		std::vector<MonoClass*> classes;

		for (uint32_t i = 0; i < rows; i++)
		{
			MonoClass* monoClass = nullptr;
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);

			monoClass = Mono::GetMonoClass(image, nameSpace, name);

			std::stringstream ss;
			ss << nameSpace << "." << name;
			HZR_ASSERT(monoClass, "Class is null");

			if (ScriptCache::CacheClass(ss.str(), monoClass))
				classes.push_back(monoClass);
		}

		if (registerScripts)
		{
			LoadScripts(classes);
		}

		data.Release();

		return true;
	}
	MonoImage* ScriptAssembly::GetImage() const
	{
		return mono_assembly_get_image(m_Assembly);
	}
	void ScriptAssembly::LoadScripts(const std::vector<MonoClass*> classes)
	{
		for (MonoClass* klass : classes)
		{
			ScriptMetadata script(klass);
			m_Scripts[script.GetName()] = script;
		}
	}
}