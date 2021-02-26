#pragma once

#include <hzreditor.h>
#include "MenuBar.h"

#include "GUI/Library/Layout.h"

using namespace WindowLayout;

namespace WindowElement {

	MenuBar::MenuBar()
	{
	}
	MenuBar::~MenuBar()
	{
	}
	void MenuBar::OnMenuRender()
	{
		ImGui::BeginMenuBar();

		if (Layout::MenuItem("Element")) {
			
		}

		ImGui::EndMenuBar();
	}
}