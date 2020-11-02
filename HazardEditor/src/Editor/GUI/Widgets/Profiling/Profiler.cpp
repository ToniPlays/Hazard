#pragma once
#include <hzreditor.h>
#include "Profiler.h"

using namespace Hazard;

Profiler::Profiler() : Layer("Profiler") {}

Profiler::~Profiler()
{
	delete name;
	logger = nullptr;
	delete logger;
}

bool Profiler::OnEnabled()
{
	logger = ModuleHandler::GetModule<Logger>();
	return logger != nullptr;
}

void Profiler::Render()
{

	if (!Panel::Begin(name, isLayerOpen)) return;
	ProfiledFn* root = ModuleHandler::GetModule<Logger>()->GetProfiledFn();

	if (root == nullptr) {
		Panel::End();
		return;
	}
	for (ProfiledFn* fn : root->subFunctions) DisplayLog(fn);

	Panel::End();
}

void Profiler::DisplayLog(ProfiledFn* parent)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth
		| ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_AllowItemOverlap;

	std::stringstream ss;
	ss << parent->name << ": ";
	ss << parent->timeInMillis;

	if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)parent->name, flags, ss.str().c_str())) {
		for (ProfiledFn* fn : parent->subFunctions)
			DisplayLog(fn);
		ImGui::TreePop();
	}
}
