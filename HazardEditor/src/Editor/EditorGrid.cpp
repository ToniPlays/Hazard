
#include "EditorGrid.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/Math/Time.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/RenderContext/ShaderAsset.h"

namespace Editor
{
	using namespace HazardRenderer;

	Grid::~Grid()
	{

	}

	void Grid::Render(const Editor::EditorCamera& camera)
	{
		if (!m_ShowGrid || !m_Pipeline) return;
	}

}
