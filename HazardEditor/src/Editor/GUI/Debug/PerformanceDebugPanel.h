
#include "Hazard.h"
#include "Hazard/ImGUI/UIElements/TextField.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI 
{
	class PerformanceDebugPanel : public Hazard::ImUI::Panel
	{
	public:
		PerformanceDebugPanel();

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	private:

		void DrawPerformanceTimers();
		void DrawMemoryView();
		void DrawJobSystemView();

		ImVec4 GetMemoryColor(uint64_t bytes);

		std::string ThreadStatusToString(ThreadStatus status);

	private:
		Hazard::ImUI::TextField m_SearchField;
	};
}