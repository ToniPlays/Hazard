#pragma once

#include "Console.h"

bool Console::isOpen = true;
void Console::OnRender() const {

	if (!isOpen) return;

	ImGui::Begin("Console", &isOpen);
	if (ImGui::Button("Clear")) {
		debug->Clear();
	}

	std::vector<Message*> logs = debug->getAll();

	for (Message* msg : logs) {
		ImGui::Text(msg->text.c_str());
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
