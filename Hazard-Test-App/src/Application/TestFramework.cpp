#include "TestFramework.h"
#include "Hazard/Input/InputManager.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "Hazard/RenderContext/ShaderCompiler.h"

#include "Tests/JobSystemTest.h"
#include "Tests/TriangleTest.h"
#include "Tests/TexturedQuad.h"
#include "Tests/UniformBufferTest.h"
#include "Tests/ComputeShaderTest.h"

using namespace Hazard;

void TestFramework::PreInit()
{
	ApplicationCreateInfo appInfo = {};
	appInfo.AppName = "Hazard Test";
	appInfo.Logging = true;

	RenderContextCreateInfo renderer = {};
	renderer.Renderer = HazardRenderer::RenderAPI::Auto;
	renderer.VSync = true;
	renderer.Width = 1920;
	renderer.Height = 1080;

	HazardCreateInfo info = {};
	info.AppInfo = &appInfo;
	info.RenderContextInfo = &renderer;

	CreateApplicationStack(&info);
}
void TestFramework::Init()
{
	auto& window = Application::GetModule<RenderContextManager>().GetWindow();
	window.AddDebugCallback([](const RenderMessage& msg) {
		std::cout << msg.Description << std::endl;
		std::cout << msg.StackTrace << std::endl;
	});

	BindingGroup kbGroup = {};
	kbGroup.Type = InputDeviceType::Keyboard;
	kbGroup.Axis[0].KeyCode = Key::Space;

	BindingGroup mouseGroup = {};
	mouseGroup.Type = InputDeviceType::Mouse;
	mouseGroup.Axis[0].KeyCode = Mouse::ButtonLeft;

	InputBinding binding = {};
	binding.AxisMask = InputAxisDirection_Button;
	binding.Groups = { kbGroup, mouseGroup };
	binding.Callback = [&](const InputBinding& binding, uint32_t key) {
		if (binding.IsPressed()) return;

		m_TestIndex++;
		RestartTest();
	};

	InputSchema schema = {};
	schema.Bindings = { binding };
	schema.Enabled = true;

	Application::GetModule<InputManager>().InvalidateSchema(schema);

	m_Tests.push_back(new TriangleTest());
	m_Tests.push_back(new JobGraphTest());
	m_Tests.push_back(new TexturedQuadTest());
	m_Tests.push_back(new UniformBufferTest());
	m_Tests.push_back(new ComputeShaderTest());

    GenerateShaders();
    
#ifdef HZR_PLATFORM_WINDOWS
	
#endif

	RestartTest();
}

void TestFramework::Update()
{
	if (m_CurrentTest)
		m_CurrentTest->Run();
}

void TestFramework::GenerateShaders()
{
#if defined HZR_PLATFORM_MACOS || defined HZR_PLATFORM_WINDOWS
	std::filesystem::path outputDir = "assets/compiled/shaders/";
	std::vector<RenderAPI> compileFor = { RenderAPI::Metal, RenderAPI::Vulkan, RenderAPI::OpenGL };

	std::vector<std::filesystem::path> sources = {
		"assets/shaders/triangle.glsl",
		"assets/shaders/texturedQuad.glsl",
		"assets/shaders/UboTest.glsl",
        "assets/shaders/compute.glsl"
	};

	std::unordered_map<RenderAPI, std::string> extensions = {
        { RenderAPI::OpenGL, "ogl" },
        { RenderAPI::Vulkan, "vk" },
        { RenderAPI::Metal, "mtl" }
	};
    
    
	for (auto& api : compileFor)
	{
		for (auto& source : sources)
		{
			for (auto& [type, shader] : ShaderCompiler::GetShaderSources(source))
			{
                std::string shaderType = Utils::ShaderStageToString(type);
                auto path = outputDir / std::filesystem::path(fmt::format("{0}.{1}.{2}", File::GetNameNoExt(source), shaderType, extensions[api]));
				if (File::Exists(path)) continue;

				auto shaderSourceCode = ShaderCompiler::GetShaderFromSource(type, shader, api);

				if (api == RenderAPI::Vulkan)
				{
					HZR_ASSERT(File::WriteBinaryFile(path, (void*)shaderSourceCode.c_str(), shaderSourceCode.length()), "Failed to write file");
				}
				else
				{
					HZR_ASSERT(File::WriteFile(path, shaderSourceCode), "Failed to write file");
				}
			}
		}
	}
#endif
}

void TestFramework::RestartTest()
{
	if (m_CurrentTest)
		m_CurrentTest->Terminate();

	m_CurrentTest = m_Tests[m_TestIndex % m_Tests.size()];
	std::cout << fmt::format("Running test: {0}", m_CurrentTest->GetName()) << std::endl;

	m_CurrentTest->Reset();
	m_CurrentTest->Init();
}
