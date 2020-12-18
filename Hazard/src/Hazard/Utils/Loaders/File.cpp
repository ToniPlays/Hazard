#pragma once
#include <hzrpch.h>
#include "File.h"
#include <commdlg.h>
#include <shlobj.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "Hazard/ModuleHandler.h"
#include "Hazard/Utils/StringUtils.h"


namespace Hazard {

	std::string File::OpenFileDialog(const char* filters) {
		OPENFILENAMEA ofn;
		
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)ModuleHandler::GetModule<RenderEngine>()->GetWindow().GetNativeWindow());
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
	std::string File::SaveFile(const char* filters) {
		
		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)ModuleHandler::GetModule<RenderEngine>()->GetWindow().GetNativeWindow());
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
	std::string File::SaveFolderDialog() {
		
		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)ModuleHandler::GetModule<RenderEngine>()->GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE) {
			std::vector<std::string> string = StringUtils::SplitString(ofn.lpstrFile, '\\');
			string.erase(string.end() - 1);
			std::string result = "";
			for (std::string f : string) 
				result += f + "\\";
			return result;
		}
		return "";

	}
	std::string File::ReadFile(std::string file) {

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

	std::string File::GetFileExtension(std::string file) {
		return file.substr(file.find_last_of(".") + 1);
	}
}