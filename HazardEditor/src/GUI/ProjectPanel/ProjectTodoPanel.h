
#include "Hazard.h"

namespace UI 
{
	class ProjectTodoPanel : public Hazard::ImUI::Panel
	{
	public:
		ProjectTodoPanel() : Hazard::ImUI::Panel("Project") { m_Open = false; };

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };

	private:
	};
}