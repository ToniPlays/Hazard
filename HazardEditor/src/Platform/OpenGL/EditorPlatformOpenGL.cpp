#pragma once
#include "EditorPlatformOpenGL.h"

#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "Platform/GLFW/imgui_impl_glfw.h"

EditorPlatformOpenGL::EditorPlatformOpenGL(GLFWwindow* window)
{
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

EditorPlatformOpenGL::~EditorPlatformOpenGL()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
}

void EditorPlatformOpenGL::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
}

void EditorPlatformOpenGL::EndFrame()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void EditorPlatformOpenGL::Close()
{
	ImGui::DestroyContext();
}
