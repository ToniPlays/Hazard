#pragma once

#include <hzrpch.h>
#include "WorldHandler.h"
#include "Loader/WorldDeserializer.h"
#include "Loader/WorldSerializer.h"
#include "WorldCommand.h"

namespace Hazard::ECS {

	WorldHandler::WorldHandler() : Module::Module("World handler") {}
	WorldHandler::~WorldHandler() {}

	bool WorldHandler::LoadScene(const std::string& file, Serialization type)
	{
		if (!File::Exists(file)) {
			HZR_THROW("Scene file not found: " + file);
		}
		if (type == Serialization::Editor) {
			m_World = Loader::WorldDeserializer::DeserializeEditor(file.c_str());
			return true;
		}
		return false;
	}
}