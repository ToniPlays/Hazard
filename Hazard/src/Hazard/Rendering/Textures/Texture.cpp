#pragma once

#include <hzrpch.h>
#include "Texture.h"

namespace Hazard::Rendering {

	Texture::~Texture()
	{
		HZR_CORE_INFO("Unloaded {0}", m_File);
	}
}
