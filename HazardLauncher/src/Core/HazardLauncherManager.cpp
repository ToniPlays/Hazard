#include "HazardLauncherManager.h"
#include "Hazard.h"

HazardLauncherManager::HazardLauncherManager()
{
	m_LoadedProjects = std::vector<HazardProject>();
}

bool HazardLauncherManager::OpenProject(const HazardProject& project)
{
	std::cout << "Opening Hazard project" << std::endl;

	std::stringstream ss;
	ss << "-wdir C:/dev/Hazard/HazardEditor";
	ss << " -hprj " << StringUtil::Replace((project.Path / "Project.hzrproj").string(), "\\", "/");
	std::cout << ss.str() << std::endl;
	return File::CreateSubprocess("C:/dev/Hazard/bin/Debug-windows-x86_64/HazardEditor/HazardEditor.exe", ss.str());
}

bool HazardLauncherManager::ImportProject(const std::filesystem::path& path)
{
	if (!File::Exists(path)) return false;
	if (File::GetFileExtension(path) != "hzrproj") return false;
	
	auto& parent = File::GetDirectoryOf(path);

	for (auto& project : m_LoadedProjects) {
		if (project.Path == parent) return false;
	}

	HazardProject project = {};
	YAML::Node root = YAML::LoadFile(path.string());

	if (root["General"]) {
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
	std::cout << File::GetEnvironmentVar("HAZARD_DIR") << std::endl;
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
	std::cout << "Creating project at " << project.Path.string() << std::endl;
	File::CreateDir(project.Path);
	File::Copy("res/TemplateProject", project.Path, CopyOptions::Recursive);
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
		std::ifstream stream(project.Path / "Library" / "Win-CreateScriptProject.bat");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::ofstream out(project.Path / "Library" / "Win-CreateScriptProject.bat");
		out << StringUtil::Replace(ss.str(), "%HAZARD_DIR%", File::GetEnvironmentVar("HAZARD_DIR"));
	}
	{
		std::ifstream stream(project.Path / "Library" / "BuildSolution.bat");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::string csProj = project.Path.string() + "\\" + (project.Name + ".csproj");
		std::ofstream out(project.Path / "Library" / "BuildSolution.bat");
		out << StringUtil::Replace(ss.str(), "%CSPROJ%", csProj);
	}

	{
		File::CreateDir(project.Path / "Assets" / "Scripts");
		File::NewFile(project.Path / "Assets" / "Scripts.meta");

		File::CreateDir(project.Path / "Assets" / "Materials");
		File::NewFile(project.Path / "Assets" / "Materials.meta");

		File::CreateDir(project.Path / "Assets" / "Sprites");
		File::NewFile(project.Path / "Assets" / "Sprites.meta");

		File::CreateDir(project.Path / "Assets" / "Models");
		File::NewFile(project.Path / "Assets" / "Models.meta");

		File::CreateDir(project.Path / "Assets" / "Worlds");
		File::NewFile(project.Path / "Assets" / "Worlds.meta");
	}

	std::filesystem::path genProjectPath = project.Path / "Library" / "Win-CreateScriptProject.bat";
	int id = File::CreateSubprocess(genProjectPath.string(), "");
	File::WaitForSubprocess(id);
	
	HZR_THREAD_DELAY(1000ms);
	{
		std::filesystem::path buildPath = project.Path / "Library" / "BuildSolution.bat";
		File::SystemCall(buildPath.string());
	}

	m_LoadedProjects.push_back(project);
	return true;
}
