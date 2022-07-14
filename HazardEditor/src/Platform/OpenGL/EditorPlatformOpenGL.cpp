
#include "EditorPlatformOpenGL.h"
#include "Backend/Core/Renderer.h"

#include "../ImGui_Backend/imgui_impl_opengl3.h"
#include "../ImGui_Backend/imgui_impl_glfw.h"

EditorPlatformOpenGL::EditorPlatformOpenGL(HazardRenderer::Window& window)
{
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window.GetNativeWindow(), true);
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
	ImGui::NewFrame();
}

void EditorPlatformOpenGL::EndFrame()
{
	HazardRenderer::Renderer::Submit([]() mutable {
		HZR_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		});
}

void EditorPlatformOpenGL::Close()
{
	ImGui::DestroyContext();
}
