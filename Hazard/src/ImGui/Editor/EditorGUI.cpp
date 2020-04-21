#pragma once
#include <hzrpch.h>
#include "EditorGUI.h"
#include "imgui.h"
#include "Items/MainMenu.h"
#include "ImGui/Editor/Widgets/Performance.h"
#include "ImGui/Editor/Widgets/Viewport.h"
#include "GLFW/glfw3.h"


namespace Hazard {

	ColorPicker* EditorGUI::colorPicker = nullptr;
    std::unordered_map<std::string, GuiLayer*> EditorGUI::layers;

	EditorGUI::EditorGUI() : Layer("EditorLayer")
	{

	}

	EditorGUI::~EditorGUI()
	{

	}
	void EditorGUI::OnAttach()
	{
		colorPicker = new ColorPicker();
		PushLayer("Performance", new Performance());
        PushLayer("Viewport", new Viewport());
	}

	void EditorGUI::OnDetach()
	{
		for (std::pair<std::string, GuiLayer*> layer : layers) {
			delete layer.second;
		}
	}

	void EditorGUI::OnImGuiRender()
    {
        //Docking stuff
        {
            static bool open = true;
            bool opt_fullscreen = true;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiDockNodeFlags_PassthruCentralNode;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
                window_flags |= ImGuiWindowFlags_NoBackground;
            }

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &open, window_flags);

            MainMenu::OnRender();
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            ImGui::End();
        }

        for (std::pair<std::string, GuiLayer*> layer : layers) {
            layer.second->OnRender();
        }
        if (colorPicker->isOpen) colorPicker->OnRender();
    }
	void EditorGUI::OpenPicker(GuiLayer* layer, Color color = Color())
	{
		colorPicker->getterLayer = layer;
		colorPicker->color = color;
		colorPicker->isOpen = true;
	}
    void EditorGUI::OpenLayer(std::string name)
    {
        layers.at(name)->OpenLayer();
    }
}

