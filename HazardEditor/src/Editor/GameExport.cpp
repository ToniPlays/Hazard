#pragma once
#include <hzreditor.h>
#include "GameExport.h"

void GameExport::ExportGame(const std::string& file, std::vector<ECS::World*> worlds)
{
	std::thread exportThread(Export, file, worlds);
	exportThread.join();
	HZR_WARN("Game exported: {0}", file);
}

void GameExport::Export(const std::string& file, std::vector<ECS::World*> worlds)
{
	File::CopyFileTo("res\\shaders\\compiled\\standard_vert.glsl", file + "res\\shaders\\standard_vert.glsl");
	File::CopyFileTo("res\\shaders\\compiled\\standard_frag.glsl", file + "res\\shaders\\standard_frag.glsl");

	for (auto world : worlds) {
		HZR_CORE_INFO("Exporting {0}", world->GetWorldFile());
	}
}
