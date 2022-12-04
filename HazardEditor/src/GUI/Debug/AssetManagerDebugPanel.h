
#include "Hazard.h"
#include "Hazard/Assets/Asset.h"

struct ImColor;

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
		ImColor GetLoadStateColor(const Hazard::LoadState& state);

	private:
		std::string m_SearchValue;
	};
}