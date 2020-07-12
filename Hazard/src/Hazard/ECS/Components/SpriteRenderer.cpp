#pragma once

#include <hzrpch.h>
#include "SpriteRenderer.h"
#include "Hazard/Utils/Loaders/MeshLoader.h"

namespace Hazard {
	
	SpriteRenderer::SpriteRenderer(GameObject* gameObject, Mesh* mesh) : Component(gameObject, "SpriteRenderer")
	{
		this->mesh = mesh;
	}

	SpriteRenderer::~SpriteRenderer()
	{
		delete mesh;
	}

}