#pragma once

#include <hzreditor.h>
#include "Profiler.h"

#include <ImSequencer.h>
#include "GUI/Library/Input.h"
#include "nlohmann/json.hpp"

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)


namespace WindowElement {

	Profiler::Profiler() : EditorWindow(ICON_FK_COMPASS " Profiler") {}

	void Profiler::Init()
	{
		SetActive(false);
	}
	void Profiler::OnWindowRender()
	{
		if (Input::Button("Load")) {
			std::string file = File::OpenFileDialog("");
			if (file != "") {
				GenerateSequence(file);
			}
		}
		if (m_Data.size() == 0) return;

		for (auto instrument : m_Data) {
			std::stringstream ss;
			ss << instrument.name << " duration: " << instrument.duration / 1000.0f << "ms";
			ImGui::Text(ss.str().c_str());
		}
	}
	void Profiler::GenerateSequence(const std::string& file)
	{
		using namespace nlohmann;
		std::string source = File::ReadFile(file);
		auto obj = json::parse(source)["traceEvents"];
		
		m_Data.clear();

		for (auto instrument : obj) {
			if (!instrument.contains("name")) continue;
			m_Data.push_back({ instrument["name"], instrument["dur"] });
		}
	}
}