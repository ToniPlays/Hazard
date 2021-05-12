#pragma once

#include <hzrpch.h>
#include "Texture.h"
#include "../RenderUtils.h"

namespace Hazard::Rendering {

	Texture::~Texture()
	{
		RenderUtils::RemoveAsset((Texture*)this);
	}
}
