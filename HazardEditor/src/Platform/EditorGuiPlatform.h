#pragma once

#include "Hazard.h"
using namespace Hazard;

class EditorGuiPlatform {

public:
	EditorGuiPlatform() = default;
	virtual ~EditorGuiPlatform() = default;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Close() = 0;
	virtual void AddTexture(Ref<Rendering::Image> texture) = 0;
};