#include "OpenGLUtils.h"

namespace HazardRenderer::OpenGLUtils 
{
	uint32_t GetMipLevelCount(uint32_t width, uint32_t height)
	{
		return (uint32_t)std::floor(std::log2(glm::min(width, height))) + 1;
	}
}
