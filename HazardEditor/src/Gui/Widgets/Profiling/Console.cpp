#pragma once

#include "Console.h"
#include <sstream>

bool Console::isOpen = true;
bool Console::showErrors = true;
bool Console::showWarnings = true;
bool Console::showInfo = true;

void Console::OnRender() const {

	if (!isOpen) return;

	//std::vector<Message*> logs = debug->getAll();
	const char* types[3] = { "Log", "Warning", "Error" };
	const char* colors[3] = { "#b9b9b9", "#DCB540", "#B20E0E" };

	ImGui::Begin("Console", &isOpen);
	/*if (ImGui::Button("Clear")) {
		debug->Clear();
	}*/
	ImGui::SameLine();
	ImGui::Checkbox("Info", &showInfo);

	ImGui::SameLine();
	ImGui::Checkbox("Warnings", &showWarnings);

	ImGui::SameLine();
	ImGui::Checkbox("Errors", &showErrors);
	ImGui::SameLine(NULL, 100);

	std::stringstream ss;
	//ss << "Logs: " << logs.size();
	ImGui::Text(ss.str().c_str());

	ss.str("");
	/*for (Message* msg : logs) {

		if (msg->level == 2 && !showErrors) continue;
		if (msg->level == 1 && !showWarnings) continue;
		if (msg->level == 0 && !showInfo) continue;

		ss << msg->time << ": " << types[msg->level] << ": " << msg->text.c_str();
		ImGui::TextColored(ColorAsImVec(Hazard::Color::fromHEX(colors[msg->level])), ss.str().c_str());
		ss.str("");
	}*/
	ImGui::End();
}

void Console::OpenLayer() const
{
	//Debug::Log("Opened Console");
	isOpen = true;
}

void Console::CloseLayer() const
{
	//Debug::Log("Closed Console");
	isOpen = false;
}
