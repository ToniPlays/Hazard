#pragma once

#include "Platform/GLFW/FontAwesome.h"
#include "Profiler.h"
#include "Library/Input.h"

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

using namespace Hazard;

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
			if (!file.empty()) {
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

	}
}