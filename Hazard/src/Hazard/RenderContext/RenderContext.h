#pragma once
#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"


namespace Hazard::Rendering {

	enum class RenderAPI { 
		Auto = 0,
		OpenGL,
		Vulkan,
		DX11,
		DX12
	};

	class RenderContext : public Module::Module {

	public:
		RenderContext();
		~RenderContext();

		void InitContext(RenderAPI api = RenderAPI::Auto);
		void Update() override;
		void Close();

		void SetClearColor(Color color) { m_ClearColor = color; };

		RenderAPI GetCurrentAPI() { return m_CurrentAPI; }
		GraphicsContext& GetContext() { return *m_Window->GetContext(); }
		Window& GetWindow() { return *m_Window; }

		static const char* APIToString(RenderAPI api);

	private:
		void Process(Event& e);

	private:
		RenderAPI m_CurrentAPI;
		Window* m_Window = nullptr;
		Color m_ClearColor;

	};
}