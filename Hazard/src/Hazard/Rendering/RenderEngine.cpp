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
		bool found = false;
		Core::HazardLoop::GetModule<RenderContext>(found);
		HZR_CORE_ASSERT(found, "RenderEngine cannot start without RenderContext");
		SetActive(found);
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