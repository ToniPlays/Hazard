#pragma once

#include <hzrpch.h>
#include "Hazard/ECS/ECS.h"
#include "Hazard/Modules/Renderer/RenderEngine.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"

namespace Hazard {

	MeshRenderer::MeshRenderer() : Component("Mesh Renderer")
	{

	}

	MeshRenderer::~MeshRenderer()
	{
	}
	void MeshRenderer::SerializeComponent(YAML::Emitter& out)
	{

	}
	void MeshRenderer::DeserializeComponent(YAML::Node in)
	{

	}
}