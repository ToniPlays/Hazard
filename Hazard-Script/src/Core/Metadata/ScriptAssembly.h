#pragma once

#include "ScriptMetadata.h"
#include "Ref.h"
#include "Coral/Assembly.hpp"
#include "Coral/HostInstance.hpp"

namespace HazardScript 
{
	struct ReferencedAssembly 
	{
		std::string Name;
	};

	class ScriptAssembly : public RefCount
	{
	public:
		ScriptAssembly() = default;
		~ScriptAssembly() {};

		bool LoadAssembly(Coral::HostInstance& host, Coral::AssemblyLoadContext& context);
		
		void AddInternalCall(std::string_view className, std::string_view functionName, void* funcPtr);
		void UploadInternalCalls();

		std::string GetName() const { return File::GetNameNoExt(m_Path); }
		std::string_view GetQualifiedName() const { return m_Assembly.GetName(); }

		Coral::TypeId GetTypeByName(std::string_view typeName) const { return m_Assembly.GetTypeId(typeName); }

		void SetSourcePath(const std::filesystem::path& path) { m_Path = path; }
		std::filesystem::path GetSourcePath() const { return m_Path; }

		std::vector<ReferencedAssembly>& GetReferencedAssemblies() { return m_ReferencedAssemblies; }

		bool HasScript(const std::string& name) const
		{
			for (auto& type : m_Assembly.GetTypes())
			{
				if (type.FullName.ToString() == name)
					return true;
			}
			return false;
		}

		ScriptMetadata GetScript(const std::string& name) const
		{
			for (auto& type : m_Assembly.GetTypes())
			{
				if (type.FullName.ToString() == name)
					return ScriptMetadata(m_Host, type);
			}
			return ScriptMetadata();
		}

		template<typename T>
		std::vector<Ref<ScriptMetadata>> ViewAttributes() 
		{
			std::vector<Ref<ScriptMetadata>> results;

			for (Coral::ReflectionType type : m_Assembly.GetTypes())
			{
				//if (script->Has<T>())
				//	results.push_back(script);
			}
			return results;
		}

	private:
		std::filesystem::path m_Path;
		Coral::ManagedAssembly m_Assembly;
		Coral::HostInstance m_Host;

		std::vector<ReferencedAssembly> m_ReferencedAssemblies;
	};
}
