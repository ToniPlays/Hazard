
#include "HazardRenderer.h"
#include "Core/UIElement.h"
#include "HazardUI.h"
#include "CommandQueue.h"

#include <iostream>

using namespace HazardRenderer;
using namespace HazardUtility;
using namespace HazardUI;

namespace SimpleCanvas {

	//OpenGL: Running
	static void OnDebugInfo(RenderMessage message)
	{
		std::cout << message.Message << std::endl;
	}

	static void CreateCanvas(Ref<Canvas> canvas)
	{
		Pivot pivot = { 0.5f, 0.5f };
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = Anchor::TopLeft;
			rt.Pivot = { 0.0f, 1.0f };
			rt.Left = 75;
			rt.Top = 75;
			rt.Width = 100;
			rt.Height = 100;
		}
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = { 0.2f, 1.0f, 0.2f, 1.0f };
			rt.Pivot = { 0.5f, 1.0f };
			rt.Top = 75;
			rt.Width = 100;
			rt.Height = 100;
		}
		return;
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = Anchor::TopRight;
			rt.Pivot = { 1.0f, 1.0f };
			rt.Top = 50;
			rt.Width = 100;
			rt.Height = 100;
		}
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = Anchor::MiddleLeft;
			rt.Pivot = { 0.0f, 0.5f };
			rt.Width = 100;
			rt.Height = 100;
		}
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = Anchor::MiddleCenter;
			rt.Pivot = { 0.5f, 0.5f };
			rt.Width = 100;
			rt.Height = 100;
		}
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = Anchor::MiddleRight;
			rt.Pivot = { 1.0f, 0.5f };
			rt.Width = 100;
			rt.Height = 100;
		}
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = Anchor::BottomLeft;
			rt.Pivot = { 0.0f, 0.0f };
			rt.Width = 100;
			rt.Height = 100;
		}
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = Anchor::BottomCenter;
			rt.Pivot = { 0.5f, 0.0f };
			rt.Width = 100;
			rt.Height = 100;
		}
		{
			UIElement element = canvas->CreateElement("TestButton");
			RectTransform& rt = element.GetComponent<RectTransform>();
			rt._color = Color::White;
			rt.Anchor = Anchor::BottomRight;
			rt.Pivot = { 1.0f, 0.0f };
			rt.Width = 100;
			rt.Height = 100;
		}
	}

	static void Run()
	{
		std::vector<const char*> icons = { "res/world.png", "res/world.png" };

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Simple canvas";
		appInfo.BuildVersion = "0.0.1a";
		appInfo.Icons = icons.data();
		appInfo.IconCount = 2;
		appInfo.MessageCallback = OnDebugInfo;

		HazardRendererCreateInfo createInfo = {};
		createInfo.Renderer = RenderAPI::OpenGL;
		createInfo.Width = 1280;
		createInfo.Height = 720;
		createInfo.Maximized = false;
		createInfo.FullScreen = false;
		createInfo.VSync = false;
		createInfo.Color = { 0.1f, 0.1f, 0.125f, 1.0f };
		createInfo.AppInfo = &appInfo;

		Window* window = Window::Create(&createInfo);
		window->Show();

		std::cout << "Selected device " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;
		std::cout << "Size: " << sizeof(GUIVertex) << std::endl;

		Ref<Canvas> canvas = Ref<Canvas>::Create();
		canvas->SetCanvasResolution(window->GetWidth(), window->GetHeight());
		CreateCanvas(canvas);


		Ref<CanvasRenderer> renderer = Ref<CanvasRenderer>::Create();
		renderer->SetCanvas(canvas);

		BufferLayout layout = { { "a_ScreenPos",	ShaderDataType::Float3	},
								{ "a_Color",		ShaderDataType::Float4, },
								{ "a_TexCoord",		ShaderDataType::Float	}
		};

		uint32_t* indices = new uint32_t[1500];
		uint32_t offset = 0;

		for (size_t i = 0; i < 1500; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		VertexBufferCreateInfo vboInfo = {};
		vboInfo.DebugName = "CanvasVertexBuffer";
		vboInfo.Usage = BufferUsage::DynamicDraw;
		vboInfo.Layout = &layout;
		vboInfo.Size = 250 * sizeof(GUIVertex);

		IndexBufferCreateInfo iboInfo = {};
		iboInfo.DebugName = "CanvasIndexBuffer";
		iboInfo.Usage = BufferUsage::StaticDraw;
		iboInfo.Size = 1500;
		iboInfo.Data = indices;

		PipelineSpecification spec = {};
		spec.DebugName = "CanvasRenderer";
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.ShaderPath = "standard_gui.glsl";
		spec.DrawType = DrawType::Fill;
		spec.CullMode = CullMode::None;

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vboInfo);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&iboInfo);
		Ref<Pipeline> guiPipeline = Pipeline::Create(&spec);
		Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::CreateFromSwapchain("Main");

		delete[] indices;

		while (true)
		{
			canvas->SetCanvasResolution(window->GetWidth(), window->GetHeight());
			renderer->GenerateRawDrawable();
			Batch<GUIVertex>& batch = renderer->GetRawDrawable().Batch;
			uint32_t dataSize = batch.GetDataSize();
			vertexBuffer->SetData(batch.GetData(), dataSize);

			window->BeginFrame();
			cmdBuffer->Begin();

			vertexBuffer->Bind(cmdBuffer);
			indexBuffer->Bind(cmdBuffer);
			guiPipeline->Bind(cmdBuffer);
			guiPipeline->Draw(cmdBuffer, batch.GetIndexCount());

			cmdBuffer->End();
			cmdBuffer->Submit();

			window->Present();
		}
	}
}
