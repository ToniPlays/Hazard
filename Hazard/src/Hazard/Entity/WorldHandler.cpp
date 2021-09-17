#pragma once

#include <hzrpch.h>
#include "WorldHandler.h"
#include "Loader/WorldDeserializer.h"
#include "Loader/WorldSerializer.h"
#include "WorldCommand.h"

namespace Hazard::ECS {

	WorldHandler::WorldHandler() : Module::Module("World handler") {}
	WorldHandler::~WorldHandler() {}

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
		m_World->SetBackground(WorldBackground::Colored, "");

		Entity entity = m_World->CreateEntity("Camera");
		entity.AddComponent<CameraComponent>();

		return true;
	}
}