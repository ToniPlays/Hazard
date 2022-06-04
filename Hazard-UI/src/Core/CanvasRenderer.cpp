
#include "CanvasRenderer.h"
#include "Batch.h"
#include "Color.h"
#include "UIElement.h"
#include "Components/Components.h"

namespace HazardUI
{
	CanvasRenderer::CanvasRenderer()
	{
		m_RawDrawable.Batch = Batch<GUIVertex>(1500);
	}

	void CanvasRenderer::GenerateRawDrawable()
	{
		m_DrawList.Clear();

		float unitWidth = m_RenderableCanvas->GetCanvasResolution().x;
		float unitHeight = m_RenderableCanvas->GetCanvasResolution().y;

		//Preprocess data

		auto view = m_RenderableCanvas->GetElementsWith<RectTransform>();
		for (auto rt : view) {
			UIElement element = { rt, m_RenderableCanvas };
			RectTransform& transform = element.GetComponent<RectTransform>();

			GuiDrawable drawable;
			drawable.Color = transform._color;
			drawable.FinalBounds = CalculateFinalBounds(transform, unitWidth, unitHeight);
			//Translate by Pivot

			m_DrawList.Add(drawable);
		}

		//Wat me do?

		//Generate the actual thing

		m_RawDrawable.Batch.Reset();

		for (GuiDrawable drawable : m_DrawList.Drawables) {

			GUIVertex bottomLeft = {};
			bottomLeft.Position = { drawable.FinalBounds.MinX * 2.0f - 1.0f, drawable.FinalBounds.MinY * 2.0f - 1.0f, 0.0f };
			bottomLeft.Color = drawable.Color;
			bottomLeft.TextureIndex = 0.0f;

			GUIVertex topLeft = {};
			topLeft.Position = { drawable.FinalBounds.MinX * 2.0f - 1.0f, drawable.FinalBounds.MaxY * 2.0f - 1.0f, 0.0f };
			topLeft.Color = drawable.Color;
			topLeft.TextureIndex = 0.0f;

			GUIVertex topRight = {};
			topRight.Position = { drawable.FinalBounds.MaxX * 2.0f - 1.0f, drawable.FinalBounds.MaxY * 2.0f - 1.0f, 0.0f };
			topRight.Color = drawable.Color;
			topRight.TextureIndex = 0.0f;

			GUIVertex bottomRight = {};
			bottomRight.Position = { drawable.FinalBounds.MaxX * 2.0f - 1.0f, drawable.FinalBounds.MinY * 2.0f - 1.0f, 0.0f };
			bottomRight.Color = drawable.Color;
			bottomRight.TextureIndex = 0.0f;

			m_RawDrawable.Batch.Push(bottomLeft);  // 0
			m_RawDrawable.Batch.Push(topLeft);	   // 1
			m_RawDrawable.Batch.Push(topRight);    // 2
			m_RawDrawable.Batch.Push(bottomRight); // 3

			m_RawDrawable.Batch.AddIndices(6);
		}
	}
	Anchor CanvasRenderer::CalculateFinalBounds(RectTransform& transform, float canvasWidth, float canvasHeight)
	{
		Anchor anchor;

		//Calculate Width
		{
			float unitWidth = transform.Width / canvasWidth;
			if (transform.Anchor.MinX == transform.Anchor.MaxX)
			{

				float paddingLeft = transform.Left / canvasWidth;
				float paddingRight = transform.Right / canvasWidth;

				float pivotLeft = transform.Pivot.PivotX * unitWidth + paddingLeft;
				float pivotRight = (1.0f - transform.Pivot.PivotX) * unitWidth + paddingLeft;

				anchor.MinX = transform.Anchor.MinX + pivotLeft;
				anchor.MaxX = transform.Anchor.MaxX + pivotRight;
			}
			else
			{

			}
		}
		{
			float unitHeight = transform.Height / canvasHeight;
			//Calculate Height
			if (transform.Anchor.MinY == transform.Anchor.MaxY)
			{
				float paddingTop = transform.Top / canvasHeight;
				float paddingBottom = transform.Bottom / canvasHeight;

				float pivotBottom = transform.Pivot.PivotY * unitHeight + paddingTop;
				float pivotTop = (1.0f - transform.Pivot.PivotY) * unitHeight - paddingTop;

				anchor.MinY = transform.Anchor.MinY - pivotBottom;
				anchor.MaxY = transform.Anchor.MaxY - pivotTop;
			}
			else
			{

			}
		}

		return anchor;
	}
}