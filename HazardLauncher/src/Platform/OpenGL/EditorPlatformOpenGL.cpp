
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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorPlatformOpenGL::EndFrame()
{
	m_Window->BeginFrame();

	using namespace HazardRenderer::OpenGL;
	Ref<OpenGLSwapchain> swapchain = m_Context->GetSwapchain();
	Ref<RenderCommandBuffer> cmdBuffer = swapchain->GetSwapchainBuffer();

	m_Context->BeginRenderPass(cmdBuffer, swapchain->GetRenderPass());

	Renderer::Submit([&]() mutable {

		HZR_PROFILE_FUNCTION("EditorPlatformOpenGL::EndFrame() RT");
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
	m_Context->EndRenderPass(cmdBuffer);
	m_Window->Present();
	Renderer::WaitAndRender();
}

void EditorPlatformOpenGL::Close()
{
	ImGui::DestroyContext();
}
