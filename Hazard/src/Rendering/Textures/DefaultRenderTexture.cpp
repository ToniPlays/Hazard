#include "hzrpch.h"
#include "DefaultRenderTexture.h"

namespace Hazard {
	DefaultRenderTexture::DefaultRenderTexture() : RenderTexture("Default render texture")
	{
		ID = 0;
		ClearColor = Color::FromHex("#222222");
	}
}
