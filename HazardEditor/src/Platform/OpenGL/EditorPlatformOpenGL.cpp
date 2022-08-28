
#include "EditorPlatformOpenGL.h"
#include "Backend/Core/Renderer.h"
#include "Backend/OpenGL/OpenGLSwapchain.h"

#include "../ImGui_Backend/imgui_impl_opengl3.h"
#include "../ImGui_Backend/imgui_impl_glfw.h"

using namespace HazardRenderer;

EditorPlatformOpenGL::EditorPlatformOpenGL(Window& window)
{
	m_Window = &window;
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window.GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	m_Context = (OpenGL::OpenGLContext*)window.GetContext();
}

EditorPlatformOpenGL::~EditorPlatformOpenGL()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
}

void EditorPlatformOpenGL::BeginFrame()
{
	HZR_PROFILE_FUNCTION();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorPlatformOpenGL::EndFrame()
{
	HZR_PROFILE_FUNCTION();

	auto& cmdBuffer = m_Context->GetSwapchain()->GetSwapchainBuffer();
	cmdBuffer->BeginRenderPass_RT(m_Context->GetSwapchain()->GetRenderPass());

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();

	cmdBuffer->EndRenderPass();

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
