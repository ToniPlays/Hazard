
#define HZR_CUSTOM_ENTRY
#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include "Hazard/Utils/File.h"
#include "Hazard/Utils/StringUtil.h"

struct CreateProjectInfo {
	std::string ProjectName;
	std::filesystem::path Path;
};

using namespace Hazard;

static void CreateProject(CreateProjectInfo* info) {
	std::cout << "Creating project: " << info->ProjectName << std::endl;
	File::Copy("./res/TemplateProject", info->Path, CopyOptions::Recursive);

	{
		std::ifstream stream(info->Path / "premake5.lua");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::string f = ss.str();

		std::ofstream out(info->Path / "premake5.lua");
		out << StringUtil::Replace(f, "%PROJECT_NAME%", info->ProjectName);
		out.close();
	}
	{
		std::ifstream stream(info->Path / "project.hzrproj");
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		std::string f = ss.str();

		std::ofstream out(info->Path / "project.hzrproj");
		out << StringUtil::Replace(f, "%PROJECT_NAME%", info->ProjectName);
		out.close();
	}
	//Run premake script
	std::string batchFilePath = info->Path.string();
	std::replace(batchFilePath.begin(), batchFilePath.end(), '/', '\\');
	batchFilePath += "\\Win-CreateScriptProject.bat";
	system(batchFilePath.c_str());
}

int main() {

	CreateProjectInfo info = {};

	std::cout << "Create project" << std::endl;

	info.Path = File::SaveFile("");
	info.ProjectName = File::GetName(info.Path.string());


	if (!info.Path.empty())
	{
		CreateProject(&info);
	}

	return 0;
}
