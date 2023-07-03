#pragma region

#include "Hazard.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI
{
	class ProjectSettingsPanel : public Hazard::ImUI::Panel
	{
	public:
		ProjectSettingsPanel() : Hazard::ImUI::Panel("Project Settings", false) {};
		~ProjectSettingsPanel() = default;

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };

	private:
		void DrawInputSettingsPanel();
	};
}