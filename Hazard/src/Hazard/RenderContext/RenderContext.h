#pragma once
#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"


namespace Hazard::Rendering {

	enum class RenderAPI { 
		Auto = 0,
		OpenGL
	};

	class RenderContext : public Module::Module {

	public:
		RenderContext(RenderAPI api = RenderAPI::OpenGL);
		~RenderContext();

		void Init();
		void Update() override;
		void Close();

		void SetClearColor(Color color) { m_ClearColor = color; };
		RenderAPI GetCurrentAPI() { return m_CurrentAPI; }
		GraphicsContext& GetContext() { return *m_Window->GetContext(); }
		Window& GetWindow() { return *m_Window; }

	private:
		void Process(Event& e);

	private:
		RenderAPI m_CurrentAPI;
		Window* m_Window = nullptr;
		Color m_ClearColor;

	};
}