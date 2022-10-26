
#include "Hazard.h"

namespace UI 
{
	class PerformanceDebugPanel : public Hazard::ImUI::Panel
	{
	public:
		PerformanceDebugPanel() : Hazard::ImUI::Panel("PerformanceDebugPanel") { m_Open = true; };

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	private:
		void DrawMemoryView();
	};
}