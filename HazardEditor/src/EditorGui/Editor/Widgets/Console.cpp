#pragma once

#include "Console.h"
#include <sstream>

bool Console::isOpen = true;
void Console::OnRender() const {

	if (!isOpen) return;

	ImGui::Begin("Console", &isOpen);
	if (ImGui::Button("Clear")) {
		debug->Clear();
	}

	std::vector<Message*> logs = debug->getAll();
	const char* types[3] = { "Log", "Warning", "Errpr" };
	for (Message* msg : logs) {
		std::stringstream ss;
		ss << types[msg->level] << ": " << msg->text.c_str();
		ImGui::Text(ss.str().c_str());
	}
	ImGui::End();

}

void Console::OpenLayer() const
{
	Debug::Log("Opened Console");
	isOpen = true;
}

void Console::CloseLayer() const
{
	Debug::Log("Closed Console");
	isOpen = false;
}
