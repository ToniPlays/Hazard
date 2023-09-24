#pragma once

#include "UtilityCore.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/Asset.h"
#include "Directory.h"

#include "ProjectSettingsSchema.h"

class HazardProject
{
public:

	HazardProject() = default;
	HazardProject(const std::filesystem::path& path);
	bool IsValid() { return !m_Info.ProjectPath.empty(); }

	HazardProjectInfo& GetInfo() { return m_Info; }
	ProjectInputSettings& GetInputSettings() { return m_InputSettings; }
	ScriptingSettings& GetScriptingSettings() { return m_ScriptSettings; }

	void ProcessAssets();
	void ProcessAsset(const std::filesystem::path& path);

	template<typename T>
	bool SerializeSetting()
	{
		HZR_ASSERT(false, "");
		return false;
	}

	template<>
	bool SerializeSetting<HazardProjectInfo>()
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		YamlUtils::Map(out, "Project", [&]() {
			YamlUtils::Serialize(out, "Project name", m_Info.ProjectName);
			YamlUtils::Serialize(out, "Version", m_Info.ProjectVersion);
			YamlUtils::Serialize(out, "Startup world", m_Info.StartupWorld);
		});
		out << YAML::EndMap;

		return File::WriteFile(m_Info.ProjectPath / "Project.hzrproj", out.c_str());
	}
	template<>
	bool SerializeSetting<ProjectInputSettings>()
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::EndMap;
		return File::WriteFile(m_Info.ProjectPath / "Project" / "InputSettings.yml", out.c_str());
	}
	template<>
	bool SerializeSetting<ScriptingSettings>()
	{
		YAML::Emitter out;
		out << YAML::BeginMap;

		YamlUtils::Sequence(out, "Preprocessors", [&]() {
			for (auto& define : m_ScriptSettings.Defines)
				out << define;
		});

		out << YAML::EndMap;
		return File::WriteFile(m_Info.ProjectPath / "Project" / "ScriptingSettings.yml", out.c_str());
	}

	template<typename T>
	T& DeserializeSetting()
	{
		HZR_ASSERT(false, "");
		return T();
	}
	template<>
	HazardProjectInfo& DeserializeSetting()
	{
		HZR_ASSERT(!m_Info.ProjectPath.empty(), "Project path must be set");

		YAML::Node root = YAML::LoadFile(m_Info.ProjectPath.string());
		YAML::Node node = root["Project"];
		YamlUtils::Deserialize(node, "Project name", m_Info.ProjectName, std::string("New project"));
		YamlUtils::Deserialize(node, "Version", m_Info.ProjectVersion, std::string("0"));
		YamlUtils::Deserialize(node, "Startup world", m_Info.StartupWorld, std::filesystem::path(""));

		if (!File::IsDirectory(m_Info.ProjectPath))
			m_Info.ProjectPath = File::GetDirectoryOf(m_Info.ProjectPath);

		return m_Info;
	}
	template<>
	ProjectInputSettings& DeserializeSetting()
	{
		auto filePath = m_Info.ProjectPath / "Project" / "InputSettings.yml";
		if (!File::Exists(filePath))
			return m_InputSettings;

		YAML::Node root = YAML::LoadFile(filePath.string());
		return m_InputSettings;
	}
	template<>
	ScriptingSettings& DeserializeSetting()
	{
		auto filePath = m_Info.ProjectPath / "Project" / "ScriptingSettings.yml";
		if (!File::Exists(filePath))
			return m_ScriptSettings;

		YAML::Node root = YAML::LoadFile(filePath.string());
		if (root["Preprocessors"])
		{
			for (auto node : root["Preprocessors"])
				m_ScriptSettings.Defines.push_back(node.as<std::string>());
		}

		return m_ScriptSettings;
	}

private:
	HazardProjectInfo m_Info;
	ProjectInputSettings m_InputSettings;
	ScriptingSettings m_ScriptSettings;
};