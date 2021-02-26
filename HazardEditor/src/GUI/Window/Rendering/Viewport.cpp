#pragma once

#include <hzreditor.h>
#include "Viewport.h"
#include "GUI/Library/Layout.h"


namespace WindowElement {

	Viewport::Viewport() : EditorWindow("Viewport")
	{

	}
	Viewport::~Viewport()
	{

	}
	void Viewport::Init()
	{

	}
	void Viewport::OnWindowRender()
	{
		WindowLayout::Layout::Text("Viewport");
	}
}