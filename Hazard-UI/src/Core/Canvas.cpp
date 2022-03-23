
#include "Canvas.h"
#include "UIElement.h"

namespace HazardUI 
{
	Canvas::Canvas()
	{

	}
	Canvas::~Canvas()
	{

	}
	UIElement Canvas::CreateElement(const std::string& name) 
	{
		UIElement element = { m_Registry.create(), this };
		element.AddComponent<RectTransform>();
		return element;
	}
	void Canvas::SetCanvasResolution(float w, float h)
	{
		m_CanvasSize.x = w;
		m_CanvasSize.y = h;
	}
}