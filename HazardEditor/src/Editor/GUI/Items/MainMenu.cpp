#pragma once
#include <hzreditor.h>
#include "Editor/GUI/All.h"
#include "MainMenu.h"
#include "Editor/Core/HazardProject.h"


void MainMenu::OnCreate() {
	MenuItems* file = new MenuItems("File");
	file->PushLayer(new Callback("Open file", [](void* item) {
		Hazard::File::OpenFileDialog("Hazard Scene (*.hazard)\0*.hazard\0");
	}, NULL, "Ctrl+O"));
	file->PushLayer(new Callback("Open project", [](void* item) {
		HazardProject::OpenProject(Hazard::File::OpenFileDialog("Hazard Project (*.hzrproj)\0*.hzrproj\0"));
	}, NULL));
	file->PushLayer(new Callback("Save Scene", [](void* item) {
		HazardProject::SaveCurrentScene();
	}, NULL));
	menuItems.push_back(file);
	
	menuItems.push_back(new MenuItems("Edit"));
	menuItems.push_back(new MenuItems("Assets"));
	menuItems.push_back(new MenuItems("Objects"));
	menuItems.push_back(new MenuItems("Component"));
	menuItems.push_back(new MenuItems("View"));
	menuItems.push_back(new MenuItems("Help"));
}

void MainMenu::OnRender()
{
	ImGui::BeginMenuBar();

	for (MenuItems* menu : menuItems) {
		RenderMenu(menu);
	}
	ImGui::EndMenuBar();
}
void MainMenu::RenderMenu(MenuItems* menu) {
	
	if (ImGui::BeginMenu(menu->label.c_str())) {

		for (Callback* callback : menu->callbacks) {
			if (ImGui::MenuItem(callback->label.c_str(), callback->shortcut)) {
				callback->fn(callback->item);
			}
		}

		for (MenuItems* item : menu->menuItems) {
			RenderMenu(item);
		}
		ImGui::EndMenu();
	}
}
void MainMenu::RegisterMenuItem(std::string str, Callback* callback) {

	std::vector<std::string> menuString = StringUtils::SplitString(str, '/');
	MenuItems* currentMenu = nullptr;
	
	for (uint8_t i = 0; i < menuString.size(); i++) {
		currentMenu = FindMenu(currentMenu, menuString.at(i));
	}
	currentMenu->PushLayer(callback);
}
MenuItems* MainMenu::FindMenu(MenuItems* menu, std::string& label) {
	if (menu == nullptr) {
		for (MenuItems* m : menuItems) {
			if (m->label == label) return m;
		}
		MenuItems* newMenu = new MenuItems(label);
		menuItems.push_back(newMenu);
		return newMenu;
	}

	for (MenuItems* childMenu : menu->menuItems) {
		if (childMenu->label == label) return childMenu;
	}

	MenuItems* newMenu = new MenuItems(label);
	menu->PushMenuItem(newMenu);
	return newMenu;
}


