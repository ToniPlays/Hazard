
#include "CanvasRenderer.h"
#include "Batch.h"

using namespace HazardUtility;

namespace HazardUI
{
	CanvasRenderer::CanvasRenderer() 
	{
		m_RawDrawable.Batch = Batch<GUIVertex>(1500);
	}

	void CanvasRenderer::GenerateRawDrawable()
	{
		m_DrawList.Clear();

		GuiDrawable drawable = {};
		drawable.Pivot = { 0.5f, 0.5f };
		drawable.Pivot = { 0.5f, 0.5f };
		drawable.Anchor = { 0.5f, 0.5f, 0.5f, 0.5f };

		m_DrawList.Add(drawable);

		//Generate the actual thing

		m_RawDrawable.Batch.Reset();

		for (GuiDrawable drawable : m_DrawList.Drawables) {

			GUIVertex bottomLeft = {};
			bottomLeft.Position = { -0.5f, -0.5f, 0.0f };
			bottomLeft.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			bottomLeft.TextureIndex = 0.0f;

			GUIVertex topLeft = {};
			topLeft.Position = {  0.5f, -0.5f, 0.0f };
			topLeft.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			topLeft.TextureIndex = 0.0f;

			GUIVertex topRight = {};
			topRight.Position = { 0.5f, 0.5f, 0.0f };
			topRight.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			topRight.TextureIndex = 0.0f;


			GUIVertex bottomRight = {};
			bottomRight.Position = { -0.5f, 0.5f, 0.0f };
			bottomRight.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			bottomRight.TextureIndex = 0.0f;

			m_RawDrawable.Batch.Push(bottomLeft);
			m_RawDrawable.Batch.Push(bottomRight);
			m_RawDrawable.Batch.Push(topRight);
			m_RawDrawable.Batch.Push(topLeft);

			m_RawDrawable.Batch.AddIndices(6);
		}
	}
}