#pragma once
#include <hzrpch.h>
#include "File.h"

#include "Hazard/Core/PlatrofmUtils.h"
#include "StringUtil.h"

#include <shlobj.h>

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

	bool File::Exists(const std::filesystem::path& path)
	{
		return std::filesystem::exists(path);
	}
	bool File::DirectoryExists(const std::filesystem::path& dir)
	{
		return std::filesystem::exists(dir);
	}
	void File::WriteFile(const std::string& dest, const std::string& source)
	{
		std::ofstream out(dest);
		out << source;
		out.close();
	}
	bool File::WriteBinaryFile(const std::filesystem::path& path, std::vector<uint32_t> data)
	{
		HZR_CORE_INFO("Writing file: {0}", path.string());
		std::ofstream out(path, std::ios::out | std::ios::binary);

		if (out.is_open()) {
			out.write((char*)data.data(), data.size() * sizeof(uint32_t));
			out.flush();
			out.close();
			return true;
		}
		return false;
	}
	bool File::WriteBinaryFile(const std::filesystem::path& path, void* data, size_t size)
	{
		HZR_CORE_INFO("Writing file: {0}", path.string());
		std::ofstream out(path, std::ios::out | std::ios::binary);

		if (out.is_open()) {
			out.write((char*)data, size);
			out.flush();
			out.close();
			return true;
		}
		return false;
	}
	bool File::CopyFileTo(const std::string& source, const std::string& dest) {

		std::string destFolder = GetDirectoryOf(dest);
		if (!DirectoryExists(destFolder)) {
			File::CreateDir(destFolder);
		}
		return std::filesystem::copy_file(source, dest);
	}
	bool File::IsNewerThan(const std::filesystem::path& file, const std::filesystem::path& compareTo)
	{
		auto fTime = std::filesystem::last_write_time(file);
		auto sTime = std::filesystem::last_write_time(compareTo);
		return fTime > sTime;
	}
	std::string File::ReadFile(const std::filesystem::path& file)
	{
		std::string result;
		std::ifstream ifs(file, std::ios::in | std::ios::binary);

		HZR_CORE_ASSERT(ifs.is_open(), "File {0} could not be opened", file.string());

		ifs.seekg(0, std::ios::end);
		size_t size = ifs.tellg();
		result.resize(size);
		ifs.seekg(0, std::ios::beg);
		ifs.read(&result[0], size);

		return result;
	}
	std::vector<char> File::ReadBinaryFile(const std::string& path)
	{
		std::ifstream stream(path, std::ios::binary | std::ios::ate);

		HZR_CORE_ASSERT(stream.is_open(), "File {0} could not be opened", path);

		auto end = stream.tellg();
		stream.seekg(0, std::ios::beg);

		auto size = std::size_t(end - stream.tellg());
		if (size == 0) return {};

		std::vector<char> buffer(size);
		if (!stream.read((char*)buffer.data(), buffer.size()))
			HZR_CORE_ERROR("Cannot read file: {0}", path);
		return buffer;
	}
	Buffer File::ReadBinaryFile(const std::filesystem::path& path)
	{
		Timer timer;
		std::ifstream stream(path, std::ios::binary | std::ios::ate);
		Buffer result;

		if (!stream.is_open())
			return result;

		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		stream.seekg(0, std::ios::beg);


		if (size == 0)
			return result;

		result.Allocate(size);

		if (!stream.read((char*)result.Data, size))
		{
			HZR_CORE_ERROR("Cannot read file: {0}", path.string());
		}

		stream.close();
		return result;
	}
	bool File::ReadBinaryFileUint32(const std::filesystem::path& path, std::vector<uint32_t>& buffer)
	{
		std::ifstream stream(path, std::ios::binary | std::ios::ate);

		if (!stream.is_open())
			return false;

		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		stream.seekg(0, std::ios::beg);

		if (size == 0)
			return false;

		buffer.resize(size / sizeof(uint32_t));

		if (!stream.read((char*)buffer.data(), size)) {
			HZR_CORE_ERROR("Cannot read file: {0}", path.string());
		}

		return true;
	}

	std::string File::GetFileAbsolutePath(const std::string& file)
	{
		std::filesystem::path path(file);
		return std::filesystem::absolute(path).string();
	}
	std::string File::GetDirectoryOf(const std::string& file)
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
	std::string File::GetPathNoExt(const std::string& file)
	{
		return file.substr(0, file.find_last_of('.'));
	}
	std::string File::GetFileExtension(const std::string& file) {
		return file.substr(file.find_last_of('.') + 1);
	}
	bool File::CreateDir(const std::filesystem::path& dir)
	{
		return std::filesystem::create_directories(dir);
	}
	FolderData File::GetFolderFiles(const std::string& folder)
	{
		FolderData result = {};
		result.Path = folder;

		std::filesystem::directory_iterator iter(folder);

		for (const auto& file : iter) {
			if (file.is_directory())
				result.Folders.emplace_back(file.path());
			else
				result.Files.emplace_back(file.path());
		}
		return result;
	}
}