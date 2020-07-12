#pragma once

#pragma once
#include <hzreditor.h>
#include "Console.h"
#include "Editor/Utils/Utility.h"

bool Console::ShowLog = true;
bool Console::ShowWarn = true;
bool Console::ShowError = true;

Console::Console()
{

}


void Console::Render()
{
	if (!isLayerOpen) return;

	std::vector<Message*> logs = Debug::GetAll();

	const char* types[3] = { "Log", "Warning", "Error" };
	const char* colors[3] = { "#b9b9b9", "#DCB540", "#B20E0E" };

	ImGui::Begin("Console", &isLayerOpen);

	if (ImGui::Button("Clear")) {
		Debug::Clear();
	}
	ImGui::SameLine();
	ImGui::Checkbox("Info", &ShowLog);

	ImGui::SameLine();
	ImGui::Checkbox("Warnings", &ShowWarn);

	ImGui::SameLine();
	ImGui::Checkbox("Errors", &ShowError);
	ImGui::SameLine(NULL, 100);

	std::stringstream ss;
	ss << "Logs: " << logs.size();
	ImGui::Text(ss.str().c_str());

	ss.str("");
	for (Message* msg : logs) {

		if (msg->level == 2 && !ShowError) continue;
		if (msg->level == 1 && !ShowWarn) continue;
		if (msg->level == 0 && !ShowLog) continue;

		ss << msg->time << "App (" << types[msg->level] << "): " << msg->text.c_str();
		ImGui::TextColored(ColorAsImVec(Hazard::Color::FromHex(colors[msg->level])), ss.str().c_str());
		ss.str("");
	}

	Hazard::Logger* logger = Hazard::ModuleHandler::GetModule<Hazard::Logger>();
	ss.str("");

	if (logger != nullptr) {
		std::vector<Hazard::Log> logs = logger->GetEngineLogs();

		ImGui::NewLine();
		ImGui::Text("Engine log");
		ImGui::SameLine(NULL, 280);

		ss << "Logs: " << logs.size();
		ImGui::Text(ss.str().c_str());
		ss.str("");

		for (Hazard::Log msg : logs) {

			int level = msg.level <= 2 ? msg.level : 2;

			if (level == 2 && !ShowError) continue;
			if (level == 1 && !ShowWarn) continue;
			if (level == 0 && !ShowLog) continue;


			ss << "Engine(" << types[level] << "): " << msg.text.c_str();
			ImGui::TextColored(ColorAsImVec(Hazard::Color::FromHex(colors[level])), ss.str().c_str());
			ss.str("");
		}
	}

	ImGui::End();
}

bool Console::OnEnabled()
{
	return true;
}

