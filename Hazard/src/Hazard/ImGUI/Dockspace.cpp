#include <hzrpch.h>

#include "Dockspace.h"
#include <GLFW/glfw3.h>
#include "imgui_internal.h"
#include "Hazard/ImGUI/UILibrary.h"

namespace Hazard::ImUI {

	void Dockspace::BeginDockspace(const char* title, ImGuiDockNodeFlags flags)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		ImGuiDockNodeFlags dockFlags = flags;

		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(title, &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);
	}
	void Dockspace::EndDockspace(const char* id)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID(id);
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
		}
		ImGui::End();
	}
	bool Dockspace::CustomTitleBar(HazardRenderer::Window& window, float height)
	{
		auto* glfwWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		const bool maximized = (bool)glfwGetWindowAttrib(glfwWindow, GLFW_MAXIMIZED);

		ImVec2 newSize, newPosition;
		if (!maximized && ImUI::UpdateWindowManualResize(ImGui::GetCurrentWindow(), newSize, newPosition))
		{
			// On Windows we hook into the GLFW win32 window internals
#ifdef HZR_PLATFORM_WINDOWS
			glfwSetWindowPos(glfwWindow, (int)newPosition.x, (int)newPosition.y);
			glfwSetWindowSize(glfwWindow, (int)newSize.x, (int)newSize.y);
#endif
		}

		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									 ImGui::GetCursorScreenPos().y + height };
		auto* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(titlebarMin, titlebarMax, ImGui::GetColorU32({ 0.5f, 1.0f, 1.0f, 1.0f }));

		ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 94;

#ifdef HZR_PLATFORM_WINDOWS

		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, height));
		bool titleBarHovered = ImGui::IsItemHovered();
		
		ImGui::EndHorizontal();

		return titleBarHovered;

#else
		auto* rootWindow = ImGui::GetCurrentWindow()->RootWindow;
		const float windowWidth = (int)rootWindow->RootWindow->Size.x;

		if (ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight), ImGuiButtonFlags_PressedOnClick))
		{
			ImVec2 point = ImGui::GetMousePos();
			ImRect rect = rootWindow->Rect();
			// Calculate the difference between the cursor pos and window pos
			moveOffsetX = point.x - rect.Min.x;
			moveOffsetY = point.y - rect.Min.y;

		}

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
			bool maximized = (bool)glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
			if (maximized)
				glfwRestoreWindow(window);
			else
				Application::Get().GetWindow().Maximize();
		}
		else if (ImGui::IsItemActive())
		{
			// TODO: move this stuff to a better place, like Window class
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{
				auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
				int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
				if (maximized)
				{
					glfwRestoreWindow(window);

					int newWidth, newHeight;
					glfwGetWindowSize(window, &newWidth, &newHeight);

					// Offset position proportionally to mouse position on titlebar
					// This ensures we dragging window relatively to cursor position on titlebar
					// correctly when window size changes
					if (windowWidth - (float)newWidth > 0.0f)
						moveOffsetX *= (float)newWidth / windowWidth;
	}

				ImVec2 point = ImGui::GetMousePos();
				glfwSetWindowPos(window, point.x - moveOffsetX, point.y - moveOffsetY);

}
		}
#endif
	}
}