#include "TestFramework.h"
#include "Hazard/Input/InputManager.h"

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
	BindingGroup kbGroup = {};
	kbGroup.Type = InputDeviceType::Keyboard;
	kbGroup.Axis[0].KeyCode = Key::Space;
    
    BindingGroup mouseGroup = {};
    mouseGroup.Type = InputDeviceType::Mouse;
    mouseGroup.Axis[0].KeyCode = Mouse::ButtonLeft;

	InputBinding binding = {};
	binding.DeviceMask = InputSource_Keyboard | InputSource_Mouse;
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

	m_Tests.push_back(new JobGraphTest());
	m_Tests.push_back(new TriangleTest());
	m_Tests.push_back(new TexturedQuadTest());
	m_Tests.push_back(new UniformBufferTest());
    m_Tests.push_back(new ComputeShaderTest());

	RestartTest();
}

void TestFramework::Update()
{
	if (m_CurrentTest)
		m_CurrentTest->Run();
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
