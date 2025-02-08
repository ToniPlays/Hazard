#include "HazardLauncherManager.h"
#include "Hazard.h"

#include "Filesystem/Directory.h"
#include "Platform/OS.h"

HazardLauncherManager::HazardLauncherManager()
{
	m_LoadedProjects = std::vector<HazardProject>();
}

bool HazardLauncherManager::OpenProject(const HazardProject& project)
{
#ifdef HZR_PLATFORM_WINDOWS
    std::string executable = "Debug-Windows-x86_64/HazardEditor/HazardEditor.exe";
#elif HZR_PLATFORM_MACOS
    std::string executable = "Debug-macosx-universal/HazardEditor/HazardEditor";
#endif
    
    std::string proc = fmt::format("{}/bin/{}", m_InstallationLocation.string(), executable);
    
	std::stringstream ss;
	ss << "-wdir ";
    ss << m_InstallationLocation.string();
    ss << "/HazardEditor";
	ss << " -hprj " << StringUtil::Replace((project.Path / "Project.hzrproj").string(), "\\", "/");
    
    HZR_INFO("Starting: {} {}", proc, ss.str());
    OS::BackgroundProcess(proc.c_str(), ss.str().c_str());
	return true;
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

	if (root["Project"]) 
	{
		YAML::Node node = root["Project"];
		YamlUtils::Deserialize<std::string>(node, "Project name", project.Name, "Unknown");
		project.Path = parent;

		m_LoadedProjects.push_back(project);
		return true;
	}
	return false;
}

bool HazardLauncherManager::LoadFromConfigFile(const std::filesystem::path& path)
{
    m_ConfigFilePath = path;
	if (!File::Exists(path)) return false;
    
    
    
	YAML::Node root = YAML::LoadFile(path.string());
    
    std::string loc;
    YamlUtils::Deserialize<std::string>(root, "Editor", loc, "");
    m_InstallationLocation = loc;
    
    YAML::Node projectNode = root["Projects"];

	for (uint64_t i = 0; i < projectNode.size(); i++) {
		auto node = projectNode[i]["Project"];
		std::string projectPath = node["Path"].as<std::string>();
		ImportProject(projectPath + "/" + "Project.hzrproj");
	}
    
	return true;
}

void HazardLauncherManager::SaveConfigToFile(const std::filesystem::path& path)
{
    m_ConfigFilePath = path;
    
	YAML::Emitter out;

	out << YAML::BeginMap;
    
    YamlUtils::Serialize(out, "Editor", m_InstallationLocation.string());

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
    
    HZR_ASSERT(File::WriteFile(path, out.c_str()), "Failed to write configs");
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
		Directory::Create(project.Path / "Assets" / "Scripts");
		Directory::Create(project.Path / "Assets" / "Materials");
		Directory::Create(project.Path / "Assets" / "Sprites");
		Directory::Create(project.Path / "Assets" / "Models");
		Directory::Create(project.Path / "Assets" / "Worlds");
		Directory::Create(project.Path / "Assets" / "Editor");
	}

	m_LoadedProjects.push_back(project);
	return true;
}

bool HazardLauncherManager::SetInstallationLocation(const std::filesystem::path &path)
{
    m_InstallationLocation = path;
    SaveConfigToFile(m_ConfigFilePath);
    return true;
}
