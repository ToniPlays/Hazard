
#include "CanvasRenderer.h"
#include "Batch.h"
#include "UIElement.h"

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

		//Preprocess data

		auto view = m_RenderableCanvas->GetElementsWith<RectTransform>();
		for (auto rt : view) {
			UIElement element = { rt, m_RenderableCanvas };
			RectTransform transform = element.GetComponent<RectTransform>();

			GuiDrawable drawable;
			drawable.Anchor = transform.Anchor;
			drawable.Pivot = transform.Pivot;

			m_DrawList.Add(drawable);
		}

		//Wat me do?

		//Generate the actual thing

		m_RawDrawable.Batch.Reset();

		for (GuiDrawable drawable : m_DrawList.Drawables) {

			GUIVertex bottomLeft = {};
			bottomLeft.Position = { drawable.Anchor.MinX * 2.0f - 1.0f, drawable.Anchor.MinY * 2.0f - 1.0f, 0.0f };
			bottomLeft.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			bottomLeft.TextureIndex = 0.0f;
			
			GUIVertex topLeft = {};
			topLeft.Position = { drawable.Anchor.MaxX * 2.0f - 1.0f, drawable.Anchor.MinY * 2.0f - 1.0f, 0.0f };
			topLeft.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			topLeft.TextureIndex = 0.0f;

			GUIVertex topRight = {};
			topRight.Position = { drawable.Anchor.MaxX * 2.0f - 1.0f, drawable.Anchor.MaxY * 2.0f - 1.0f, 0.0f };
			topRight.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			topRight.TextureIndex = 0.0f;

			GUIVertex bottomRight = {};
			bottomRight.Position = { drawable.Anchor.MinX * 2.0f - 1.0f, drawable.Anchor.MaxY * 2.0f - 1.0f, 0.0f };
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