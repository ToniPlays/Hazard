#pragma once

#include <hzreditor.h>
#include "Inspector.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout.h"

using namespace WindowLayout;

namespace WindowElement {

	Inspector::Inspector() : EditorWindow("Inspector")
	{

	}
	Inspector::~Inspector()
	{

	}
	void Inspector::OnWindowRender()
	{
		std::string text = "Some text";
		Input::InputField(text);

		if (Input::Button("Click me!")) {}

		Layout::Table(2);
		Layout::Text("Someting");
		Layout::TableNext();
		Layout::Text("Else");
		Layout::TableNext();
		Layout::Text("Here");
		Layout::TableNext();
		Layout::EndTable();

		Layout::NextLine(25.0f);

		Layout::Text("I am meme");

		static float value = 0;
		static glm::vec2 value2 = { 0, 0 };
		static glm::vec3 value3 = { 0, 0, 0 };
		
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed;
		Layout::Treenode("Transform", flags, []() {
			Layout::Text("Treenode");

			Layout::IDGroud("FloatVec", []() {
				Input::Vec1("Float", value, 0.0f, 80);
				});
			Layout::IDGroud("OffsetVec2", []() {
				Input::Vec2("Offset", value2, 0.0f, 80);
				});
			Layout::IDGroud("PositionVec3", []() {
				Input::Vec3("Position", value3, 0.0f, 80);
				});
		});
		Layout::ContextMenu([]() {
			if (Layout::MenuItem("Selected")) {
				
			}
		});
		
	}
}