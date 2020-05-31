#pragma once
#include <hzrpch.h>
#include "File.h"

namespace Hazard {

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