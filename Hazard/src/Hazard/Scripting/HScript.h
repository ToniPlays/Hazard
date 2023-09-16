#pragma once

#include "Hazard/Assets/Asset.h"
#include "HazardScript.h"

namespace Hazard 
{
	class HScript : public Asset 
	{
		friend class ScriptAssetLoader;

	public:
		HScript() = default;
		HScript(const std::string& sourceFile, const std::string& moduleName) : m_SourceFile(sourceFile), m_ModuleName(moduleName) {};
		const std::string& GetModuleName() { return m_ModuleName; }
		const std::filesystem::path& GetSourceFile() { return m_SourceFile; }

		void SetSourceFile(const std::filesystem::path& file) { m_SourceFile = file; }

	private:
		std::filesystem::path m_SourceFile;
		std::string m_ModuleName;
	};
}