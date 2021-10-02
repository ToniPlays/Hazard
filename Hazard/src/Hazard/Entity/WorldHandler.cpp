#pragma once

#include <hzrpch.h>
#include "WorldHandler.h"
#include "Loader/WorldDeserializer.h"
#include "Loader/WorldSerializer.h"
#include "WorldCommand.h"

namespace Hazard::ECS {

	WorldHandler::WorldHandler(EntityComponentCreateInfo* info) : Module::Module("World handler") {
		LoadWorld(info->StartupFile, Serialization::Editor);
	}
	WorldHandler::~WorldHandler() {}

	void WorldHandler::Close()
	{
		HZR_PROFILE_FUNCTION();
	}

	bool WorldHandler::LoadWorld(const std::string& file, Serialization type)
	{
		if (File::Exists(file)) {
			if (type == Serialization::Editor) {
				m_World = Loader::WorldDeserializer::DeserializeEditor(file.c_str());
				return true;
			}
		}
		m_World = new World("New world");
		m_World->SetName("New World");

		Entity entity = m_World->CreateEntity("Camera");
		entity.AddComponent<CameraComponent>();

		
		return false;
	}
}