#pragma once

#include "Hazard.h"

class EditorGuiPlatform {

public:
	EditorGuiPlatform() = default;
	virtual ~EditorGuiPlatform() = default;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Close() = 0;
	virtual void AddTexture(Hazard::Rendering::Texture* texture) = 0;
};