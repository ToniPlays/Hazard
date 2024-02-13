
#include "Hazard.h"
#include "Hazard/Assets/Asset.h"
#include "Hazard/ImGUI/Panel.h"

struct ImColor;

namespace UI
{
	class AssetManagerDebugPanel : public Hazard::ImUI::Panel
	{
	public:
		AssetManagerDebugPanel() : Hazard::ImUI::Panel("AssetManagerDebugPanel") {}

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };

	private:
		ImColor GetLoadStateColor(const Hazard::LoadState& state);

	private:
		std::string m_SearchValue;
	};
}