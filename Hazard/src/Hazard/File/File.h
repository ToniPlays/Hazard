#pragma once

#include <iostream>
#include <vector>

namespace Hazard::File {

	class File {
	public:
		static std::string OpenFileDialog(const std::string& filters);
		static std::string SaveFile(const std::string& filters);

		static std::string SaveFolderDialog();

		static std::string ReadFile(const std::string& file);
		static std::string GetFileExtension(const std::string& file);

		static std::vector<char> ReadBinaryFile(const char* path);
		static bool Exists(const char* file);
	};
}