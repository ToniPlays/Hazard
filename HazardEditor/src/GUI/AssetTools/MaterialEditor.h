#pragma once

#include "Hazard/ImGUI/Panel.h"
#include "Hazard/Rendering/Mesh/Material.h"

namespace UI
{
    class MaterialEditor : public Hazard::ImUI::Panel
    {
    public:
        MaterialEditor();
        ~MaterialEditor() = default;
        
        void Update() override {};
        bool OnEvent(Event& e) override { return false; };
        void OnPanelRender() override;
        
        void SetSelectedMaterial(Ref<Hazard::Material> material) { m_Material = material; }
    private:
        void DrawMaterialParam(const std::string& name, const Hazard::MaterialParam& param);
    private:
        Ref<Hazard::Material> m_Material;
    };
}
