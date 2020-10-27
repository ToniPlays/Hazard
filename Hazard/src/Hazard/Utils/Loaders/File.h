#pragma once

#include <iostream>
namespace Hazard {

	class File {
	public:
		static std::string OpenFileDialog(const char* filters);
		static std::string SaveFile(const char* filters);

		static std::string SaveFolderDialog();

		static std::string ReadFile(std::string file);
		static std::string GetFileExtension(std::string file);
	private:
		
	};
}