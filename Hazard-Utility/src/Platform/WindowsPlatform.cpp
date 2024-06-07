#include "OS.h"

#ifdef HZR_PLATFORM_WINDOWS

#include <Windows.h>
#include <commdlg.h>
#include <Psapi.h>
#include <Profiling/MemoryDiagnostic.h>
#include "File.h"
#include "Directory.h"

int OS::SysCall(const char* command) 
{
	return system(command);
}
void* OS::BackgroundProcess(const char* path, const char* arguments)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	int id = CreateProcessA(
		path,
		(LPSTR)arguments,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	);

	return (void*)id;
}
void OS::WaitForProcess(void* handle)
{

}
bool OS::HasEnv(const char* key)
{
	HKEY hKey;
	LPCSTR keyPath = "Environment";
	LSTATUS lOpenStatus = RegOpenKeyExA(HKEY_CURRENT_USER, keyPath, 0, KEY_ALL_ACCESS, &hKey);

	if (lOpenStatus == ERROR_SUCCESS)
	{
		lOpenStatus = RegQueryValueExA(hKey, key, 0, NULL, NULL, NULL);
		RegCloseKey(hKey);
	}

	return lOpenStatus == ERROR_SUCCESS;
}
const char* OS::GetEnv(const char* key)
{
	HKEY hKey;
	LPCSTR keyPath = "Environment";
	DWORD createdNewKey;
	LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);

	if (lOpenStatus == ERROR_SUCCESS)
	{
		DWORD valueType;
		char data[512];
		DWORD dataSize = 512;
		LSTATUS status = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &valueType, (PVOID)data, &dataSize);

		RegCloseKey(hKey);

		if (status == ERROR_SUCCESS)
			return data;
	}
	return NULL;


}
bool OS::SetEnv(const char* key, const char* value)
{
	HKEY hKey;
	LPCSTR keyPath = "Environment";
	DWORD createdNewKey;
	LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);

	if (lOpenStatus == ERROR_SUCCESS)
	{
		std::string val(value);
		LSTATUS lSetStatus = RegSetValueExA(hKey, key, 0, REG_SZ, (LPBYTE)val.c_str(), val.length() + 1);
		RegCloseKey(hKey);

		if (lSetStatus == ERROR_SUCCESS)
		{
			SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
			return true;
		}
	}

	return false;
}
void OS::Dialog(const char* title, const char* description)
{
	const WCHAR* pwcsName; //LPCWSTR

	int size = MultiByteToWideChar(CP_ACP, 0, description, -1, NULL, 0);
	pwcsName = hnew WCHAR[256];
	MultiByteToWideChar(CP_ACP, 0, description, -1, (LPWSTR)pwcsName, size);

	MessageBox(NULL, pwcsName, (LPCTSTR)L"Error", MB_OK);
	hdelete[] pwcsName;
}

bool OS::ShowLocation(const std::filesystem::path& path) 
{
	auto abs = File::GetFileAbsolutePath(path).lexically_normal();
	if (!File::Exists(abs)) return false;

	std::string cmd = "explorer.exe /select,\"" + abs.string() + "\"";
	return SysCall(cmd.c_str()) == 0;
}
bool OS::OpenDirectory(const std::filesystem::path& path) 
{
	auto abs = File::GetFileAbsolutePath(path);
	if (!File::Exists(path)) return false;

	ShellExecute(NULL, L"explore", abs.c_str(), NULL, NULL, SW_SHOWNORMAL);
	return true;
}
bool OS::OpenInDefault(const std::filesystem::path& path)
{
	auto abs = File::GetFileAbsolutePath(path);
	if (!File::Exists(path)) return false;

	return SysCall((std::string("explorer.exe ") + abs.string()).c_str()) == 0;
}

#endif