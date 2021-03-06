#pragma once

#include <iostream>
#include <vector>

namespace Hazard::File {

	class File {
	public:
		static std::string OpenFileDialog(const char* filters);
		static std::string SaveFile(const char* filters);

		static std::string SaveFolderDialog();

		static std::string ReadFile(std::string file);
		static std::string GetFileExtension(std::string file);

		static std::vector<char> ReadBinaryFile(const char* path);
		static bool Exists(const char* file);
	};
}