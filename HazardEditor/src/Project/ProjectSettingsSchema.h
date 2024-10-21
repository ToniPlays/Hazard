#pragma once

#include "UtilityCore.h"
#include "Utility/YamlUtils.h"
#include "File.h"
#include "Directory.h"

#include "Hazard/Assets/Asset.h"
#include "Hazard/Assets/AssetManager.h"

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

	std::filesystem::path CurrentBuildConfig;

	void Deserialize(YAML::Node& root) override {
		YamlUtils::Deserialize<std::string>(root["Project"], "Company name", CompanyName, "MyCompany");
		YamlUtils::Deserialize<std::string>(root["Project"], "Project name", ProjectName, "My Hazard Project");
		YamlUtils::Deserialize<std::string>(root["Project"], "Version", VersionIdentifier, "");
		YamlUtils::Deserialize<std::filesystem::path>(root["Build"], "Config", CurrentBuildConfig, "");
	}

	void Serialize(YAML::Emitter& out) override
	{
		YamlUtils::Map(out, "Project", [this, &out]() {
			YamlUtils::Serialize(out, "Company name", CompanyName);
			YamlUtils::Serialize(out, "Project name", ProjectName);
			YamlUtils::Serialize(out, "Version", VersionIdentifier);
		});
		YamlUtils::Map(out, "Build", [this, &out]() {
			YamlUtils::Serialize(out, "Config", CurrentBuildConfig);
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
	std::vector<Hazard::AssetMetadata> TargetWorlds;

	void Deserialize(YAML::Node& root) override
	{
		YamlUtils::Deserialize<std::filesystem::path>(root["Target"], "Directory", BuildDirectory, "Builds/");
		YamlUtils::Deserialize<std::string>(root["Target"], "Target format", BuildTargetFormat, "{project.name}");

		for(auto p : root["Worlds"])
		{
			AssetHandle handle;
			YamlUtils::Deserialize<AssetHandle>(root["Worlds"], p.first.as<std::string>(), handle, 0);
			Hazard::AssetMetadata& metadata = Hazard::AssetManager::GetMetadata(handle);

			if (metadata.Handle == INVALID_ASSET_HANDLE) continue;

			TargetWorlds.push_back(metadata);
		}
	}

	void Serialize(YAML::Emitter& out) override
	{
		YamlUtils::Map(out, "Target", [this, &out]() {
			YamlUtils::Serialize(out, "Directory", BuildDirectory);
			YamlUtils::Serialize(out, "Target format", BuildTargetFormat);
		});

		YamlUtils::Map(out, "Worlds", [this, &out]() {
			for (auto& metadata : TargetWorlds)
				YamlUtils::Serialize(out, File::GetNameNoExt(metadata.SourceFile), metadata.Handle);
		});
	}
};

struct HazardProjectInfo
{
	AppRuntimeConfig RuntimeConfig;
	RuntimeBuildSettings BuildSettings;
};
