#pragma once

#include "Hazard.h"
#include "Hazard/Core/Application.h"
#include "Core/Window.h"
#include "Platform/GUIPlatform.h"
#include "GUIRenderable.h"
#include "MenuBar.h"

namespace Hazard
{
	class GUIManager : public Hazard::Module
	{
	public:
		GUIManager() = delete;
		GUIManager(GuiCreateInfo* info) : Hazard::Module("GUIManager") {};
		~GUIManager() = default;

		void Init() override;
		void Update() override;
		void Render() override;

		bool OnEvent(Event& e) override;

		template<typename T>
		T* GetRenderable(uint32_t id)
		{
			return FindRenderable<T>(id);
		}

		template<typename T>
		T& New()
		{
			T* panel = hnew T();
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
		T& GetExistingOrNew()
		{
			ImUI::GUIRenderable* panel = FindAny<T>();
			return panel ? (T&)*panel : New<T>();
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
		ImUI::GUIRenderable* FindRenderable(uint64_t handle)
		{
			if (handle == 0) 
				return nullptr;

			auto& panels = m_Renderables[typeid(T).hash_code()];

			for (auto& panel : panels)
			{
				if (panel->GetPanelID() == handle) 
					return panel;
			}

			return nullptr;
		}

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
	};
}
