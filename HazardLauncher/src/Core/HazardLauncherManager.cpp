#include "HazardLauncherManager.h"
#include "Hazard.h"

#include "Directory.h"

HazardLauncherManager::HazardLauncherManager()
{
	m_LoadedProjects = std::vector<HazardProject>();
}

bool HazardLauncherManager::OpenProject(const HazardProject& project)
{
	std::stringstream ss;
	ss << "-wdir C:/dev/Hazard/HazardEditor";
	ss << " -hprj " << StringUtil::Replace((project.Path / "Project.hzrproj").string(), "\\", "/");
	return File::CreateSubprocess("C:/dev/Hazard/bin/Debug-windows-x86_64/HazardEditor/HazardEditor.exe", ss.str());
}

bool HazardLauncherManager::ImportProject(const std::filesystem::path& path)
{
	if (!File::Exists(path)) return false;
	if (File::GetFileExtension(path) != ".hzrproj") return false;
	
	auto parent = File::GetDirectoryOf(path);

	for (auto& project : m_LoadedProjects) 
		if (project.Path == parent) return false;

	HazardProject project = {};
	YAML::Node root = YAML::LoadFile(path.string());

	if (root["General"]) 
	{
		YAML::Node node = root["General"];
		YamlUtils::Deserialize<std::string>(node, "Project name", project.Name, "Unknown");
		project.Path = parent;

		m_LoadedProjects.push_back(project);
		return true;
	}
	return false;
}

bool HazardLauncherManager::LoadFromConfigFile(const std::filesystem::path& path)
{
	if (!File::Exists(path)) return false;

	YAML::Node root = YAML::LoadFile(path.string());
	YAML::Node projectNode = root["Projects"];

	for (size_t i = 0; i < projectNode.size(); i++) {
		auto node = projectNode[i]["Project"];
		std::string projectPath = node["Path"].as<std::string>();
		ImportProject(projectPath + "\\" + "Project.hzrproj");
	}
	return true;
}

void HazardLauncherManager::SaveConfigToFile(const std::filesystem::path& path)
{
	std::ofstream file(path);
	YAML::Emitter out;

	out << YAML::BeginMap;

	YamlUtils::Sequence(out, "Projects", [&]() {
		for (HazardProject project : m_LoadedProjects) {
			out << YAML::BeginMap;
			YamlUtils::Map(out, "Project", [&]() {
				YamlUtils::Serialize(out, "Name", project.Name);
				YamlUtils::Serialize(out, "Path", project.Path.string());
				});
			out << YAML::EndMap;
		}
		});
	out << YAML::EndMap;
	file << out.c_str();
}

bool HazardLauncherManager::CreateProject(const HazardProject& project)
{
	Directory::Create(project.Path);
	File::Copy("res/ProjectTemplate", project.Path, CopyOptions::Recursive);
	{
		std::ifstream stream(project.Path / "premake5.lua");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::ofstream out(project.Path / "Premake5.lua");
		out << StringUtil::Replace(ss.str(), "%PROJECT_NAME%", project.Name);
	}
	{
		std::ifstream stream(project.Path / "Project.hzrproj");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::ofstream out(project.Path / "Project.hzrproj");
		out << StringUtil::Replace(ss.str(), "%PROJECT_NAME%", project.Name);
	}
	{
		std::ifstream stream(project.Path / "Project" / "Win-CreateScriptProject.bat");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::ofstream out(project.Path / "Project" / "Win-CreateScriptProject.bat");
		out << StringUtil::Replace(ss.str(), "%HAZARD_DIR%", File::GetEnvironmentVar("HAZARD_DIR"));
	}
	{
		std::ifstream stream(project.Path / "Project" / "BuildSolution.bat");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::string csProj = project.Path.string() + "\\" + (project.Name + ".csproj");
		std::ofstream out(project.Path / "Project" / "BuildSolution.bat");
		out << StringUtil::Replace(ss.str(), "%CSPROJ%", csProj);
	}

	{
		Directory::Create(project.Path / "Assets" / "Scripts");
		Directory::Create(project.Path / "Assets" / "Materials");
		Directory::Create(project.Path / "Assets" / "Sprites");
		Directory::Create(project.Path / "Assets" / "Models");
		Directory::Create(project.Path / "Assets" / "Worlds");
		Directory::Create(project.Path / "Assets" / "Editor");
	}

	std::filesystem::path genProjectPath = project.Path / "Project" / "Win-CreateScriptProject.bat";
	int id = File::CreateSubprocess(genProjectPath.string(), "");
	File::WaitForSubprocess(&id);
	
	HZR_THREAD_DELAY(500ms);
	{
		std::filesystem::path buildPath = project.Path / "Project" / "BuildSolution.bat";
		File::SystemCall(buildPath.string());
	}

	m_LoadedProjects.push_back(project);
	return true;
}

std::string HazardLauncherManager::GenerateMetaFileContent(const std::filesystem::path& path, const AssetType& type)
{
	using namespace Hazard;
	YAML::Emitter out;

	out << YAML::BeginMap;
	YamlUtils::Serialize(out, "UID", AssetHandle());
	YamlUtils::Serialize(out, "Type", type);
	YamlUtils::Serialize(out, "Path", path);
	out << YAML::EndMap;
	return out.c_str();
}
