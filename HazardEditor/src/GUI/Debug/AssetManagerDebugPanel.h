
#include "Hazard.h"

namespace UI 
{
	class AssetManagerDebugPanel : public Hazard::ImUI::Panel
	{
	public:
		AssetManagerDebugPanel() : Hazard::ImUI::Panel("AssetManagerDebugPanel") { m_Open = false; };

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };
	private:
		std::string m_SearchValue;
	};
}