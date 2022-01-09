
#include <hzrpch.h>

#ifdef HZR_PLATFORM_WINDOWS
#include "Hazard/Core/PlatformUtils.h"
#include "portable-file-dialogs.h"
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
	std::string PlatformUtils::OpenFolderDialog(const std::string& title) 
	{
		return pfd::select_folder(title, "", pfd::opt::none).result();
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
	bool PlatformUtils::HasEnvVariable(const std::string& key) {
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		LSTATUS lOpenStatus = RegOpenKeyExA(HKEY_CURRENT_USER, keyPath, 0, KEY_ALL_ACCESS, &hKey);

		if (lOpenStatus == ERROR_SUCCESS)
		{
			lOpenStatus = RegQueryValueExA(hKey, key.c_str(), 0, NULL, NULL, NULL);
			RegCloseKey(hKey);
		}

		return lOpenStatus == ERROR_SUCCESS;

	}
	std::string PlatformUtils::GetEnvVariable(const std::string& key) {
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		DWORD createdNewKey;
		LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);
		if (lOpenStatus == ERROR_SUCCESS)
		{
			DWORD valueType;
			char* data = new char[512];
			DWORD dataSize = 512;
			LSTATUS status = RegGetValueA(hKey, NULL, key.c_str(), RRF_RT_ANY, &valueType, (PVOID)data, &dataSize);

			RegCloseKey(hKey);

			if (status == ERROR_SUCCESS)
			{
				std::string result(data);
				delete[] data;
				return result;
			}
		}

		return std::string{};

	}
	bool PlatformUtils::SetEnvVariable(const std::string& key, const std::string& value) {
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		DWORD createdNewKey;
		LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);
		if (lOpenStatus == ERROR_SUCCESS)
		{
			LSTATUS lSetStatus = RegSetValueExA(hKey, key.c_str(), 0, REG_SZ, (LPBYTE)value.c_str(), value.length() + 1);
			RegCloseKey(hKey);

			if (lSetStatus == ERROR_SUCCESS)
			{
				SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
				return true;
			}
		}

		return false;

	}
}
#endif
