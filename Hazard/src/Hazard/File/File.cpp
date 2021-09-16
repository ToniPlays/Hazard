#pragma once
#include <hzrpch.h>
#include "File.h"

#include <shlobj.h>
#include "Hazard/Core/PlatrofmUtils.h"
#include "StringUtil.h"

namespace Hazard {

	std::string File::OpenFileDialog(const char* filters) {
		return PlatformUtils::OpenFileDialog(filters);
	}
	std::string File::SaveFolderDialog() {
		return PlatformUtils::SaveFolderDialog();
	}
	std::string File::SaveFile(const char* filters) {
		return PlatformUtils::SaveFileDialog(filters);
	}

	bool File::Exists(const std::string& file)
	{
		std::ifstream f(file);
		return f.good();
	}
	bool File::DirectoryExists(const std::string& dir)
	{
		return std::filesystem::exists(dir);
	}
	void File::WriteFile(const std::string& dest, const std::string& source)
	{
		std::ofstream out(dest);
		out << source;
		out.close();
	}
	void File::WriteBinaryFile(const std::string& dest, std::vector<uint32_t> data)
	{
		std::ofstream out(dest);
		out.write((char*)data.data(), data.size() * sizeof(uint32_t));
		out.flush();
		out.close();
	}
	bool File::CopyFileTo(const std::string& source, const std::string& dest) {
	
		std::string destFolder = GetDirectoryOf(dest);
		if (!DirectoryExists(destFolder)) {
			File::CreateDir(destFolder);
		}
		return std::filesystem::copy_file(source, dest);
	}
	std::string File::ReadFile(const std::string& file) 
	{
		std::ifstream ifs(file, std::ios::in, std::ios::binary);

		HZR_ASSERT(ifs.is_open(), "File " + file + " could not be opened");

		std::string result;
		ifs.seekg(0, std::ios::end);
		result.resize(ifs.tellg());
		ifs.seekg(0, std::ios::beg);

		ifs.read(&result[0], result.size());
		ifs.close();

		return result;
	}
	std::vector<char> File::ReadBinaryFile(const std::string& path)
	{
		std::ifstream stream(path, std::ios::binary | std::ios::ate);

		HZR_CORE_ASSERT(stream, "Cannot open filepath: {0}", path);

		auto end = stream.tellg();
		stream.seekg(0, std::ios::beg);

		auto size = std::size_t(end - stream.tellg());
		if (size == 0) return {};

		std::vector<char> buffer(size);
		if (!stream.read((char*)buffer.data(), buffer.size()))
			HZR_CORE_ERROR("Cannot read file: {0}", path);
		return buffer;
	}
	std::vector<uint32_t> File::ReadBinaryFileUint32(const std::string& path)
	{
		std::ifstream stream(path, std::ios::binary | std::ios::ate);

		HZR_CORE_ASSERT(stream, "Cannot open filepath: {0}", path);

		auto end = stream.tellg();
		stream.seekg(0, std::ios::beg);

		auto size = std::size_t(end - stream.tellg());
		if (size == 0) return {};

		std::vector<uint32_t> buffer(size);
		if (!stream.read((char*)buffer.data(), buffer.size()))
			HZR_CORE_ERROR("Cannot read file: {0}", path);
		return buffer;
	}

	std::string File::GetFileAbsolutePath(const std::string& file)
	{
		std::filesystem::path path(file);
		return std::filesystem::absolute(path).string();
	}
	std::string Hazard::File::GetDirectoryOf(const std::string& file)
	{
		std::vector<std::string> string = StringUtil::SplitString(file, '\\');
		string.erase(string.end() - 1);

		std::string result = "";

		for (std::string f : string)
			result += f + "\\";
		return result;
	}
	std::string File::GetName(const std::string& file)
	{
		std::filesystem::path path(file);
		return path.filename().string();
	}
	std::string File::GetNameNoExt(const std::string& file)
	{
		std::string name = GetName(file);
		return name.substr(0, name.find_last_of('.'));
	}
	std::string File::GetFileExtension(const std::string& file) {
		return file.substr(file.find_last_of('.') + 1);
	}
	bool File::CreateDir(const std::string& dir)
	{
		return std::filesystem::create_directories(dir);
	}
	FolderData File::GetFolderFiles(const std::string& folder)
	{
		FolderData result;
		result.path = folder;

		for (const auto& file : directory_iterator(folder)) {
			if (file.is_directory()) 
				result.folders.emplace_back(file.path());
			else 
				result.files.emplace_back(file.path());
		}
		return result;
	}
}