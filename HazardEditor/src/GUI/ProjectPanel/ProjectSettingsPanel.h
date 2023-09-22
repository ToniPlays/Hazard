#pragma region

#include "Hazard.h"
#include "Hazard/ImGUI/Panel.h"
#include "Hazard/ImGUI/UIElements/TextField.h"

namespace UI
{
	class ProjectSettingsPanel : public Hazard::ImUI::Panel
	{
	public:
		ProjectSettingsPanel();
		~ProjectSettingsPanel() = default;

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };

	private:
		void RenderNavbar();
		void DrawInputSettingsPanel();

	private:
		Hazard::ImUI::TextField m_SearchField;
	};
}