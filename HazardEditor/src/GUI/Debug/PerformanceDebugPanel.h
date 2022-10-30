
#include "Hazard.h"

namespace UI 
{
	class PerformanceDebugPanel : public Hazard::ImUI::Panel
	{
	public:
		PerformanceDebugPanel() : Hazard::ImUI::Panel("PerformanceDebugPanel") { m_Open = false; };

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	private:
		void DrawPerformanceTimers();
		void DrawMemoryView();
		ImVec4 GetMemoryColor(size_t bytes);

	private:
		std::string m_MemorySearchVal;
	};
}