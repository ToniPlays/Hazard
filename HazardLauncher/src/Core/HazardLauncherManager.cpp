#include "HazardLauncherManager.h"
#include "Hazard.h"

bool HazardLauncherManager::OpenProject(const HazardProject& project)
{
	std::cout << "Opening Hazard project" << std::endl;

	std::stringstream ss;
	ss << "-wdir C:/dev/Hazard/HazardEditor";
	ss << " -hprj " << StringUtil::Replace(project.Path.string(), "\\", "/");
	std::cout << ss.str() << std::endl;
	File::CreateSubprocess("C:/dev/Hazard/bin/Debug-windows-x86_64/HazardEditor/HazardEditor.exe", ss.str());
	return true;
}

bool HazardLauncherManager::ImportProject(const std::filesystem::path& path)
{
	if (File::GetFileExtension(path) != "hzrproj") return false;

	HazardProject project = {};
	YAML::Node root = YAML::LoadFile(path.string());

	if (root["General"]) {
		YAML::Node node = root["General"];
		YamlUtils::Deserialize<std::string>(node, "Project name", project.Name, "Unknown");
		project.Path = path;

		m_LoadedProjects.push_back(project);
		return true;
	}
	return false;
}

bool HazardLauncherManager::LoadFromConfigFile(const std::filesystem::path& path)
{
	YAML::Node root = YAML::LoadFile(path.string());
	YAML::Node projectNode = root["Projects"];

	for (size_t i = 0; i < projectNode.size(); i++) {
		auto node = projectNode[i]["Project"];
		std::string projectPath = node["Path"].as<std::string>();
		ImportProject(projectPath);
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
