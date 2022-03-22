#if 0
#define HZR_CUSTOM_ENTRY
#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include "Hazard/Utils/File.h"
#include "Hazard/Utils/StringUtil.h"

#if _WIN32
#define PATH_PREFIX ""
#else
#define PATH_PREFIX "../../../HazardLauncher/"
#endif

struct CreateProjectInfo {
	std::string ProjectName;
	std::filesystem::path Path;
};

using namespace Hazard;

static void CreateProject(CreateProjectInfo* info) {
	std::cout << "Creating project: " << info->ProjectName << std::endl;
    std::cout << "To: " << File::GetFileAbsolutePath(PATH_PREFIX "res/TemplateProject") << std::endl;
    
	std::cout << "Copying files" << std::endl;
	File::Copy(PATH_PREFIX "res/TemplateProject", info->Path, CopyOptions::Recursive);

	{
		std::cout << "Generating premake file" << std::endl;
		std::ifstream stream(info->Path / "premake5.lua");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::ofstream out(info->Path / "premake5.lua");
		out << StringUtil::Replace(ss.str(), "%PROJECT_NAME%", info->ProjectName);
		out.flush();
		out.close();
	}
	{
		std::cout << "Generating project file" << std::endl;
		std::ifstream stream(info->Path / "project.hzrproj");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::string f = ss.str();

		std::ofstream out(info->Path / "project.hzrproj");
		out << StringUtil::Replace(f, "%PROJECT_NAME%", info->ProjectName);
		out.flush();
		out.close();
	}
	std::cout << "Building script project" << std::endl;
	//Run premake script
	std::string batchFilePath = info->Path.string();
#ifdef HZR_PLATFORM_WINDOWS
    std::replace(batchFilePath.begin(), batchFilePath.end(), '/', '\\');
	batchFilePath += "\\Win-CreateScriptProject.bat";
#else
    batchFilePath += "/Mac-CreateScriptProject.sh";
#endif
    std::cout << "Running: " << batchFilePath << std::endl;
	system(batchFilePath.c_str());
}

bool ValidatePath() 
{
	if (File::HasEnvinronmentVar("HAZARD_DIR")) return true;
	std::string hazardFolder = File::OpenFolderDialog("Where is Hazard repository located?");
	return File::SetEnvironmentVar("HAZARD_DIR", hazardFolder);
}

int main() {

	CreateProjectInfo info = {};
    
	if (!ValidatePath()) {
		std::cout << "Failed to locate Hazard";
		return 1;
	}
	std::cout << "Create project" << std::endl;

	info.Path = File::OpenFolderDialog("Save project to folder");
	info.ProjectName = File::GetName(info.Path.string());

	if (!info.Path.empty())
	{
		CreateProject(&info);
	}

	return 0;
}
#endif