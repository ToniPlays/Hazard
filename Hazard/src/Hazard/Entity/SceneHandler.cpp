#pragma once

#include <hzrpch.h>
#include "SceneHandler.h"

namespace Hazard::ECS {

	SceneHandler::SceneHandler() : Module::Module("Scene handler")
	{
	}
	SceneHandler::~SceneHandler()
	{
	}
	void SceneHandler::Init()
	{
		this->scene = new Scene();
	}
	void SceneHandler::Update()
	{
	}
	void SceneHandler::Render()
	{
	}
	void SceneHandler::LoadScene(const char* file)
	{
		
	}
}