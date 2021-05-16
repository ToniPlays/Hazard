#pragma once

#include <hzrpch.h>
#include "SceneHandler.h"
#include "SceneSerializer.h"
#include "SceneDeserializer.h"
#include "../SceneCommand.h"

namespace Hazard::ECS {

	SceneHandler::SceneHandler() : Module::Module("Scene handler") {}
	SceneHandler::~SceneHandler() {}

	bool SceneHandler::LoadScene(const char* file, Serialization type)
	{
		if (type == Serialization::Editor) {
			m_World = Loader::SceneDeserializer::DeserializeEditor(file);
			return true;
		}
		return false;
	}
}