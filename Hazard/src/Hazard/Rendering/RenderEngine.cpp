#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"

namespace Hazard::Rendering {
	RenderEngine::RenderEngine() : Module("RenderEngine")
	{
		SetActive(true);
	}
	RenderEngine::~RenderEngine()
	{
		Flush();
	}
	void RenderEngine::PreInit()
	{
	
	}
	void RenderEngine::Init()
	{

	}
	void RenderEngine::Update()
	{

	}
	void RenderEngine::Render()
	{

	}
	void RenderEngine::Flush()
	{
		delete context;
	}
}