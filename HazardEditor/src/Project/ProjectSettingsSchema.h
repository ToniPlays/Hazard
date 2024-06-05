#pragma once

#include "UtilityCore.h"
#include "Utility/YamlUtils.h"
#include "File.h"
#include "Directory.h"

enum class Platform
{
	Windows,
	MacOS,
	Linux,
	Android,
	IOS,
	XBox,
	PlayStation
};

static const char* PlatformToString(Platform platform)
{
	switch (platform)
	{
		case Platform::Windows:		return "Windows";
		case Platform::MacOS:		return "MacOS";
		case Platform::Linux:		return "Linux";
		case Platform::Android:		return "Android";
		case Platform::IOS:			return "IOS";
		case Platform::XBox:		return "XBox";
		case Platform::PlayStation:	return "PlayStation";
	}
}

struct ProjectSettingInfo
{
private:
	std::filesystem::path SourceFile;
public:

	bool Load(const std::filesystem::path& path) {
		SourceFile = path;
		
		if (!File::Exists(path)) return false;
	
		YAML::Node root = YAML::LoadFile(path.string());
		Deserialize(root);
		return true;
	}

	bool Save() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		Serialize(out);
		out << YAML::EndMap;

		if(!Directory::Exists(File::GetDirectoryOf(SourceFile)))
			Directory::Create(File::GetDirectoryOf(SourceFile));

		return File::WriteFile(SourceFile, out.c_str());
	}

	virtual void Deserialize(YAML::Node& root) = 0;
	virtual void Serialize(YAML::Emitter& out) = 0;
};

struct AppRuntimeConfig : ProjectSettingInfo
{
	std::string CompanyName;
	std::string ProjectName;
	std::string VersionIdentifier;
	std::filesystem::path ProjectPath;

	void Deserialize(YAML::Node& root) override {
		YamlUtils::Deserialize<std::string>(root["Project"], "Company name", CompanyName, "MyCompany");
		YamlUtils::Deserialize<std::string>(root["Project"], "Project name", ProjectName, "My Hazard Project");
		YamlUtils::Deserialize<std::string>(root["Project"], "Version", VersionIdentifier, "");
	}

	void Serialize(YAML::Emitter& out)
	{
		YamlUtils::Map(out, "Project", [this, &out]() {
			YamlUtils::Serialize(out, "Company name", CompanyName);
			YamlUtils::Serialize(out, "Project name", ProjectName);
			YamlUtils::Serialize(out, "Version", VersionIdentifier);
		});
	}
};

struct PlatformBuildSettings
{
	
};

struct RuntimeBuildSettings : ProjectSettingInfo
{
	std::filesystem::path BuildDirectory;
	std::string BuildTargetFormat;
	std::unordered_map<Platform, PlatformBuildSettings> Platform;

	void Deserialize(YAML::Node& root) override
	{
		YamlUtils::Deserialize<std::filesystem::path>(root["Target"], "Directory", BuildDirectory, "Builds/");
		YamlUtils::Deserialize<std::string>(root["Target"], "Target format", BuildTargetFormat, "{project.name}");
	}
	void Serialize(YAML::Emitter& out)
	{
		YamlUtils::Map(out, "Target", [this, &out]() {
			YamlUtils::Serialize(out, "Directory", BuildDirectory);
			YamlUtils::Serialize(out, "Target format", BuildTargetFormat);
		});
	}
};

struct HazardProjectInfo
{
	AppRuntimeConfig RuntimeConfig;
	RuntimeBuildSettings BuildSettings;
};