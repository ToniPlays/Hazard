#pragma once

#include <hzrpch.h>
#include "SceneHandler.h"
#include "SceneDeserializer.h"

namespace Hazard::ECS {

	SceneHandler::SceneHandler() : Module::Module("Scene handler")
	{
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
	void SceneHandler::Render()
	{

	}
	bool SceneHandler::LoadScene(const char* file, Serialization type)
	{

		HZR_WARN("Loading file " + std::string(file));
		if (type == Serialization::Editor) {
			scene = Loader::SceneDeserializer::DeserializeEditor(file);
			return true;
		}
		return false;
	}
}