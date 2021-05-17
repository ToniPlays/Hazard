#pragma once

class EditorGuiPlatform {

public:
	EditorGuiPlatform() {};
	virtual ~EditorGuiPlatform() {};

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
};