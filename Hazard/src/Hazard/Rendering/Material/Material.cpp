#pragma once

#include <hzrpch.h>
#include "Material.h"
#include "../RenderUtils.h"

namespace Hazard::Rendering {

	Material::Material(Shader* shader) : m_Shader(shader)
	{
		
	}
	Material::~Material()
	{

	}
	Material* Material::Create(const char* shader)
	{
		return new Material(RenderUtils::Create<Shader>(shader));
	}
}
