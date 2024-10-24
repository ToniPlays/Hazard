
#include "Hazard.h"
#include "Hazard/Scripting/Attributes/ClassAttributes.h"

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
		void DrawElement(const std::string& className, const std::string& name, const Ref<Hazard::TodoAttribute> attrib);

	private:
		std::string m_SearchVal;
	};
}