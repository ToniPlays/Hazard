#pragma region

#include "Hazard.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI
{
	class ProjectSettingsPanel : public Hazard::ImUI::Panel
	{
	public:
		ProjectSettingsPanel() : Hazard::ImUI::Panel("Project Settings") {};
		~ProjectSettingsPanel() = default;

		void OnPanelRenderer();

	private:
		void DrawInputSettingsPanel();
	};
}