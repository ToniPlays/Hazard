#pragma once

namespace HazardRenderer {
	class Window;
}

class EditorGuiPlatform {

public:
	EditorGuiPlatform() = default;
	virtual ~EditorGuiPlatform() = default;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Close() = 0;

	virtual HazardRenderer::Window& GetWindow() = 0;
};