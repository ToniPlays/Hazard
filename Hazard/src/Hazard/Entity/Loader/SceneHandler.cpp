#pragma once

#include <hzrpch.h>
#include "SceneHandler.h"
#include "SceneSerializer.h"
#include "../SceneCommand.h"

namespace Hazard::ECS {

	SceneHandler::SceneHandler() : Module::Module("Scene handler")
	{
		std::cout << "SceneHandler()" << std::endl;
	}
	SceneHandler::~SceneHandler()
	{

	}
	void SceneHandler::Init()
	{

	}
	void SceneHandler::Update()
	{

	}
	//Load scene from file
	bool SceneHandler::LoadScene(const char* file, Serialization type)
	{
		HZR_WARN("Loading file " + std::string(file));
		if (type == Serialization::Editor) {
			scene = Loader::SceneSerializer::DeserializeEditor(file);
			return true;
		}
		return false;
	}
}