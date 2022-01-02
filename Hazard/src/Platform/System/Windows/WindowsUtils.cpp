
#include <hzrpch.h>

#ifdef HZR_PLATFORM_WINDOWS
#include "Hazard/Core/PlatformUtils.h"

#include <commdlg.h>

namespace Hazard 
{
	void PlatformUtils::Messagebox(const char* title, const char* description)
	{
		const WCHAR* pwcsName; //LPCWSTR

		// required size
		int size = MultiByteToWideChar(CP_ACP, 0, description, -1, NULL, 0);
		// allocate it
		pwcsName = new WCHAR[256];
		MultiByteToWideChar(CP_ACP, 0, description, -1, (LPWSTR)pwcsName, size);
		// use it....

		MessageBox(NULL, pwcsName, (LPCTSTR)L"Error", MB_OK);
		delete[] pwcsName;
	}
	std::string PlatformUtils::OpenFileDialog(const char* filters)
	{
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
	std::string PlatformUtils::SaveFileDialog(const char* filters) 
	{
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
	std::string PlatformUtils::SaveFolderDialog()
	{
		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE) {

			return File::GetDirectoryOf(ofn.lpstrFile);
		}
		return "";
	}
}
#endif
