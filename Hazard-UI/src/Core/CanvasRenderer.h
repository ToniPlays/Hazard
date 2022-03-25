#pragma once


#include "GuiDrawList.h"
#include "Canvas.h"

#include "Ref.h"

namespace HazardUI 
{
	class CanvasRenderer : public RefCount
	{
	public:

		CanvasRenderer();
		~CanvasRenderer() {};

		void GenerateRawDrawable();

		Ref<Canvas> GetCanvas() { return m_RenderableCanvas; }
		void SetCanvas(Ref<Canvas> canvas) { m_RenderableCanvas = canvas; };

		Anchor CalculateFinalBounds(RectTransform& transform, float canvasWidth, float canvasHeight);

		GuiDrawList& GetDrawList() { return m_DrawList; }
		GuiRawDrawable& GetRawDrawable() { return m_RawDrawable; }

	private:
		GuiDrawList m_DrawList;
		GuiRawDrawable m_RawDrawable;
		Ref<Canvas> m_RenderableCanvas;

	};
}