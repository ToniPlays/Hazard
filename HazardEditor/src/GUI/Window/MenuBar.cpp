#pragma once

#include <hzreditor.h>
#include "MenuBar.h"

#include "GUI/Library/Layout.h"
#include "Project/ProjectManager.h"

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

		Layout::Menu("File", []() {
			Layout::MenuItem("New", []() {});
			Layout::MenuItem("Open", []() {});
			Layout::Separator();
			Layout::MenuItem("Save", []() {
				Hazard::Core::HazardLoop::GetModule<Project::ProjectManager>().Save();
			});
			Layout::MenuItem("Save as", []() {});
			Layout::MenuItem("New scene", []() {});
		});
		Layout::Menu("Edit", []() {
			
		});
		Layout::Menu("View", []() {
			
		});
		Layout::Menu("Assets", []() {
			
		});
		Layout::Menu("Window", []() {
			
		});
		Layout::Menu("Help", []() {
			
		});

		ImGui::EndMenuBar();
	}
}