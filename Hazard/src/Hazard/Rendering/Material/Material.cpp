#pragma once

#include <hzrpch.h>
#include "Material.h"
#include "../RenderUtils.h"

namespace Hazard::Rendering {

	Material::Material(const Ref<Shader>& shader)
	{
		shader.Raw();
		m_Shader = shader;
	}
	Material::~Material()
	{

	}
	Ref<Material> Material::Create(const MaterialCreateInfo& info)
	{
		return Ref<Material>::Create(RenderUtils::Create<Shader>(*info.shader));
	}
}
