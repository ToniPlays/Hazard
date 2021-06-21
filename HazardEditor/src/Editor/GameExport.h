#pragma once

#include "Hazard.h"

class GameExport {
public:
	static void ExportGame(const std::string& file, std::vector<ECS::World*> worlds);
private:
	static void Export(const std::string& file, std::vector<ECS::World*>& worlds);
	uint32_t progress;
};