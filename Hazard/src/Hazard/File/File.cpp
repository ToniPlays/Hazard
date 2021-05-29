#pragma once
#include <hzrpch.h>
#include "File.h"
#include <commdlg.h>

#include <shlobj.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "StringUtil.h"

namespace Hazard {

	std::string File::OpenFileDialog(const char* filters) {
		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filters;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE) {
			return ofn.lpstrFile;
		}
		return "";
	}
	std::string File::SaveFolderDialog() {

		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE) {
			
			return GetDirectoryOf(ofn.lpstrFile);
		}
		return "";

	}
	std::string File::SaveFile(const char* filters) {

		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filters;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE) {
			return ofn.lpstrFile;
		}
		return "";
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
		return file.substr(file.find_last_of(".") + 1);
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