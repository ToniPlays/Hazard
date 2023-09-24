#pragma once
#include "UtilityCore.h"

struct HazardProjectInfo
{
	std::string ProjectName;
	std::string ProjectVersion;
	std::filesystem::path ProjectPath;
	std::filesystem::path StartupWorld;
};
struct ProjectInputSettings
{
	float random = 0.0f;
};
struct ScriptDependency
{
	std::string Platform;
};
struct ScriptExecution
{
	float none = 0.0f;
};

struct ScriptingSettings
{
	std::vector<std::string> Defines;
	std::vector<ScriptDependency> Dependencies;
	std::vector<ScriptExecution> ExecutionOrder;
};