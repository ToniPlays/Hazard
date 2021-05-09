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

	std::string File::OpenFileDialog(const std::string& filters) {
		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filters.c_str();
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
			std::vector<std::string> string = StringUtil::SplitString(ofn.lpstrFile, '\\');
			string.erase(string.end() - 1);

			std::string result = "";

			for (std::string f : string)
				result += f + "\\";
			return result;
		}
		return "";

	}
	std::string File::SaveFile(const std::string& filters) {

		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filters.c_str();
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE) {
			return ofn.lpstrFile;
		}
		return "";
	}

	bool File::Exists(const char* file)
	{
		std::ifstream f(file);
		return f.good();
	}
	std::string File::ReadFile(const std::string& file) {

		std::ifstream ifs(file, std::ios::in, std::ios::binary);

		HZR_ASSERT(!ifs.is_open(), "File " + file + " could not be opened");

		std::string result;
		ifs.seekg(0, std::ios::end);
		result.resize(ifs.tellg());
		ifs.seekg(0, std::ios::beg);

		ifs.read(&result[0], result.size());
		ifs.close();
		return result;
	}
	std::vector<char> File::ReadBinaryFile(const char* path)
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

	std::string File::GetFileAbsolutePath(const std::string& file)
	{
		std::filesystem::path path(file);
		return std::filesystem::absolute(path).string();
	}
	std::string File::GetFileExtension(const std::string& file) {
		return file.substr(file.find_last_of(".") + 1);
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