#pragma once

#include "Hazard.h"
#include "GUIRenderable.h"
#include "HazardRendererCore.h"
#include "Platform/EditorGuiPlatform.h"

class GUIManager : public Hazard::Module 
{
public:
	GUIManager() : Hazard::Module("GUIManager") {};
	~GUIManager() = default;

	void Init() override;
	void Update() override;
	void Render() override;

private:
	void InitImGuiPlatform(HazardRenderer::Window& window);

private:
	std::vector<GUIRenderable> m_Renderables;
	EditorGuiPlatform* m_Platform;
	HazardRenderer::Window* m_Window;
};