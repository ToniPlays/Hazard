#pragma once
#include "Hazard.h"

using namespace Hazard;

struct Callback {

	std::string label;
	const char* shortcut = "";
	void(*fn)(void* item);
	void* item;
	Callback() = default;

	Callback(std::string label, void(*fn)(void* item), void* item, const char* shortcut = "") {
		this->label = label;
		this->fn = fn;
		this->item = item;
		this->shortcut = shortcut;
	}

};

class MenuItems {
public:
	std::string label = "#HIDDEN";
	std::vector<MenuItems*> menuItems;
	std::vector<Callback*> callbacks;

	MenuItems() = default;
	MenuItems(std::string label) {
		this->label = label;
	}
	void PushLayer(Callback* callback) { callbacks.push_back(callback); }
	void PushMenuItem(MenuItems* items) { menuItems.push_back(items); }
};

class MainMenu {

public:
	void OnCreate();
	void OnRender();
	void RegisterMenuItem(std::string str, Callback* callback);
private:

	void RenderMenu(MenuItems* menu);

	MenuItems* FindMenu(MenuItems* menu, std::string& label);
	std::vector<MenuItems*> menuItems;
};
