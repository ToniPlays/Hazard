#pragma once

#include "Hazard/ImGUI/UIElements/Table.h"
#include "Hazard/ImGUI/UIElements/Dropdown.h"
#include "Hazard/ImGUI/ScopedVar.h"
#include <Hazard/Input/InputSchema.h>
#include <Hazard/Input/InputManager.h>
#include <Editor/GUI/ProjectSettings/ProjectSettingsPanel.h>

namespace UI
{
	static uint32_t GetSelectionFromAxis(uint32_t flags)
	{
		using namespace Hazard;
		if (flags & InputAxisDirection_Vertical && flags & InputAxisDirection_Horizontal && flags & InputAxisDirection_Depth)
			return 2;
		if (flags & InputAxisDirection_Vertical && flags & InputAxisDirection_Horizontal)
			return 1;
		if (flags & InputAxisDirection_Vertical)
			return 0;
	}
	static std::string AxisDirectionToString(uint32_t direction)
	{
		using namespace Hazard;
		switch (direction)
		{
			case InputAxisDirection_None:		return "None";
			case InputAxisDirection_Button:		return "Button";
			case InputAxisDirection_Horizontal:	return "Horizontal";
			case InputAxisDirection_Vertical:	return "Vertical";
			case InputAxisDirection_Depth:		return "Depth";
		}
		HZR_ASSERT(false, "Failure");
	}

	static std::string KeyBindingsToString(Hazard::InputDeviceType type, uint32_t keycode)
	{
		switch (type)
		{
			case Hazard::InputDeviceType::None:		return "No device";
			case Hazard::InputDeviceType::Keyboard:	return Key::ToString(keycode);
			case Hazard::InputDeviceType::Mouse:	return Mouse::ToString(keycode);
			case Hazard::InputDeviceType::Gamepad:	return Gamepad::ToString(keycode);
		}
	}

	static void DrawInputSourceTable()
	{
		using namespace Hazard;

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		ImUI::Table<Ref<InputDevice>> inputSources("Input sources", { ImGui::GetContentRegionAvail().x, 0.0f });
		inputSources.RowHeight(34.0f);
		inputSources.SetColumns({ "Name", "Type", "Binding" });
		inputSources.RowContent([&](uint32_t, const Ref<Hazard::InputDevice>& device) {
			ImUI::Separator({ 4.0, 34.0f }, device->IsConnected() ? style.Colors.AxisY : style.Colors.AxisX);
			ImGui::SameLine();
			ImGui::Text("%s", device->GetName().c_str());

			ImGui::TableNextColumn();
			ImUI::ShiftX(8.0f);
			ImGui::Text("%s", InputDeviceToString(device->GetType()));

			ImGui::TableNextColumn();
			ImUI::ShiftX(8.0f);
			ImGui::Text("%u", device->GetBindingPoint());
		});

		for (auto& [type, devices] : InputManager::GetDevices())
		{
			for (auto& device : devices)
				inputSources.AddRow(device);
		}

		inputSources.Render();
	}
	static void DrawInputBindingsTable(std::vector<Hazard::InputBinding>& bindings, ProjectSettingsPanel* settings)
	{
		using namespace Hazard;

		for (uint32_t i = 0; i < bindings.size(); i++)
		{
			auto& binding = bindings[i];
			if (!(binding.AxisMask & InputAxisDirection_Button))
				continue;

			ImUI::Treenode bindingNode(binding.Name, true);
			bindingNode.DefaultOpen();
			bindingNode.Content([&binding, settings]() mutable {

				ImUI::TextField nameField(binding.Name);
				nameField.SetHint("Action name");
				nameField.Render();

				if (nameField.DidChange())
					binding.Name = nameField.GetValue();

				ImVec2 size = ImGui::GetContentRegionAvail();

				struct InputButtonBinding
				{
					InputBinding* Parent;
					BindingGroup* Group;
					InputAxis* Axis;
				};

				ImUI::Table<InputButtonBinding> bindingTable("BindingTable", ImVec2(size.x, 150), false);
				bindingTable.SetColumns({ "Type", "Mapped keys", "Inverted",  "Pressed" });
				bindingTable.RowHeight(32.0f);
				bindingTable.RowContent([settings](uint32_t, InputButtonBinding map) {
					ImUI::Shift(4.0f, 4.0f);
					ImGui::Text("%s", AxisDirectionToString(map.Axis->Direction).c_str());

					ImGui::TableNextColumn();
					ImUI::Shift(4.0f, 4.0f);

					ImGui::Text("%s", KeyBindingsToString(map.Group->Type, map.Axis->KeyCode).c_str());
					if (ImGui::IsItemClicked())
						settings->WaitForInput(map.Group->Type, map.Axis);

					ImGui::TableNextColumn();
					ImUI::Shift(4.0f, 4.0f);
					ImGui::Checkbox("##inverted", &map.Axis->Inverted);

					ImGui::TableNextColumn();
					ImUI::Shift(4.0f, 4.0f);
					ImGui::Text("%s", map.Axis->Pressed() ? "Pressed" : "Released");
				});

				for (auto& group : binding.Groups)
				{
					bindingTable.AddRow({ &binding, &group, &group.Axis[0] });
				}

				bindingTable.Render();
			});
			bindingNode.Menu((const char*)ICON_FK_LIST_UL, []() {
				ImUI::MenuItem("Remove", []() {});
			});
			bindingNode.Render();
		}
	}

	static void DrawInputAxisTable(std::vector<Hazard::InputBinding>& bindings, ProjectSettingsPanel* settings)
	{
		using namespace Hazard;

		for (uint32_t i = 0; i < bindings.size(); i++)
		{
			auto& binding = bindings[i];
			if (binding.AxisMask & InputAxisDirection_Button)
				continue;

			ImUI::Treenode bindingNode(binding.Name, true);
			bindingNode.DefaultOpen();
			bindingNode.Content([&binding]() mutable {

				ImUI::TextField nameField(binding.Name);
				nameField.SetHint("Axis name");
				nameField.Render();

				if (nameField.DidChange())
					binding.Name = nameField.GetValue();

				ImUI::Dropdown axisType("Type");
				axisType.SetOptions({ "Axis1D", "Axis2D", "Axis3D" });
				axisType.SetSelected(GetSelectionFromAxis(binding.AxisMask));
				axisType.Render();

				if (axisType.DidChange())
				{
					switch (axisType.GetSelected())
					{
						case 0: binding.AxisMask = InputAxisDirection_Vertical; break;
						case 1: binding.AxisMask = InputAxisDirection_Vertical | InputAxisDirection_Horizontal; break;
						case 2: binding.AxisMask = InputAxisDirection_Vertical | InputAxisDirection_Horizontal | InputAxisDirection_Depth; break;
					}
				}

				struct InputButtonBinding
				{
					InputBinding* Parent;
					BindingGroup* Group;
					InputAxis* Axis;
				};

				ImVec2 size = ImGui::GetContentRegionAvail();

				ImUI::Table<InputButtonBinding> bindingTable("BindingTable", ImVec2(size.x, 0.0f), false);
				bindingTable.SetColumns({  "Type", "Mapped keys", "Multiplier", "Inverted", "Current value" });
				bindingTable.RowHeight(32.0f);
				bindingTable.RowContent([bindingTable](uint32_t, InputButtonBinding& map) {
					ImUI::Shift(4.0f, 4.0f);
					ImGui::Text("%s", AxisDirectionToString(map.Axis[0].Direction).c_str());

					ImGui::TableNextColumn();
					ImUI::Shift(4.0f, 4.0f);
					ImGui::Text("%s", KeyBindingsToString(map.Group->Type, map.Axis[0].KeyCode).c_str());

					ImGui::TableNextColumn();
					ImUI::Shift(4.0f, 4.0f);
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					ImGui::DragFloat("##mult", &map.Axis[0].Multiplier, 0.1f, 0.0f, 0.0f, "%0.2f", 1.0f);

					ImGui::TableNextColumn();
					ImUI::Shift(4.0f, 4.0f);
					ImGui::Checkbox("##inverted", &map.Axis[0].Inverted);

					ImGui::TableNextColumn();
					ImUI::Shift(4.0f, 4.0f);
					ImGui::Text("%.1f", map.Axis[0].GetValueWithMultiplier());
				});

				for (auto& group : binding.Groups)
				{
					if (binding.AxisMask & InputAxisDirection_Horizontal)
						bindingTable.AddRow({&binding, &group, &group.Axis[0] });
					if (binding.AxisMask & InputAxisDirection_Vertical)
						bindingTable.AddRow({&binding, &group, &group.Axis[1] });
					if (binding.AxisMask & InputAxisDirection_Depth)
						bindingTable.AddRow({&binding, &group, &group.Axis[2] });
				}

				bindingTable.Render();
			});

			bindingNode.Menu((const char*)ICON_FK_LIST_UL, []() {
				ImUI::MenuItem("Remove", []() {});
			});
			bindingNode.Render();
		}
	}
}
