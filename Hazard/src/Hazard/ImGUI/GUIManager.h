#pragma once

#include "Hazard.h"
#include "Hazard/Core/Application.h"
#include "Core/Window.h"
#include "Platform/GUIPlatform.h"
#include "GUIRenderable.h"
#include "MenuBar.h"
#include "Panel.h"

namespace Hazard
{
	class GUIManager : public Hazard::Module
	{
	public:
		GUIManager() = delete;
		GUIManager(GuiCreateInfo* info) : Hazard::Module("GUIManager") {
			m_HasDockspace = info->DockSpace;
		};
		~GUIManager() = default;

		void Init() override;
		void Update() override;
		void Render() override;

		bool OnEvent(Event& e) override;

		template<typename T>
		T& New(const std::string& id = "")
		{
			T* panel = hnew T();
			if constexpr (std::is_base_of<ImUI::Panel, T>::value)
			{
				if (!id.empty())
					((ImUI::Panel*)panel)->SetTitle(id);
			}

			m_Renderables[typeid(T).hash_code()].push_back(panel);
			return *panel;
		}

		void Destroy(ImUI::GUIRenderable* panel)
		{
			for (auto& [type, panels] : m_Renderables)
			{
				auto it = std::find(panels.begin(), panels.end(), panel);
				if (it != panels.end())
				{
					panels.erase(it);
					return;
				}
			}
		}

		template<typename T>
		T& GetExistingOrNew(const std::string& id = "")
		{
			if constexpr (std::is_base_of<ImUI::Panel, T>::value)
			{
				//Multiple panels are possible

				auto& panels = m_Renderables[typeid(ImUI::Panel).hash_code()];
				for (auto renderable : panels)
				{
					auto& panel = *(ImUI::Panel*)renderable;
					if (panel.GetTitle() == id)
						return (T&)*renderable;
				}
				return New<T>(id);
			}

			ImUI::GUIRenderable* renderable = FindAny<T>();
			if (!renderable)
				return New<T>(id);

			return renderable ? (T&)*renderable : New<T>(id);
		}

		template<typename T>
		void SetMenubar()
		{
			if (m_Menubar)
				hdelete m_Menubar;

			m_Menubar = hnew T();
		}

		Ref<HazardRenderer::Sampler>& GetImageSampler() { return m_ImageSampler; }

	private:
		void InitImGuiPlatform(HazardRenderer::Window& window);

		template<typename T>
		ImUI::GUIRenderable* FindAny()
		{
			auto& panels = m_Renderables[typeid(T).hash_code()];
			return panels.size() != 0 ? panels[0] : nullptr;
		}

	private:
		Ref<HazardRenderer::Sampler> m_ImageSampler = nullptr;
		HazardRenderer::Window* m_Window = nullptr;

		GUIPlatform* m_Platform = nullptr;
		ImUI::MenuBar* m_Menubar = nullptr;
		std::unordered_map<uint64_t, std::vector<ImUI::GUIRenderable*>> m_Renderables;

		bool m_HasDockspace = false;
	};
}
