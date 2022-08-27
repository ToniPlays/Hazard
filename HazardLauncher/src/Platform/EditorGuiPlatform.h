#pragma once

class EditorGuiPlatform {

public:
	EditorGuiPlatform() = default;
	virtual ~EditorGuiPlatform() = default;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Close() = 0;
};