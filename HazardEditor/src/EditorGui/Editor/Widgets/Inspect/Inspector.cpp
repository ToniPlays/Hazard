#include "Inspector.h"


bool Inspector::isOpen = true;
void Inspector::OnRender() const {

	if (!isOpen) return;
	ImGui::Begin("Inspector", &isOpen);

	ImGui::End();
}
void Inspector::OpenLayer() const
{
	Debug::Log("Opened Inspector");
	isOpen = true;
}

void Inspector::CloseLayer() const
{
	Debug::Log("Closed Inspector");
	isOpen = false;
}