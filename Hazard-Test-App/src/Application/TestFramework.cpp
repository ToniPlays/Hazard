#include "TestFramework.h"

void TestFramework::PreInit()
{
    using namespace Hazard;
    
    ApplicationCreateInfo appInfo = {};
    appInfo.AppName = "Hazard Test";
    appInfo.Logging = true;
    
    RenderContextCreateInfo renderer = {};
    renderer.Renderer = HazardRenderer::RenderAPI::OpenGL;
    renderer.VSync = true;
    
    HazardCreateInfo info = {};
    info.AppInfo = &appInfo;
    info.RenderContextInfo = &renderer;
    
    CreateApplicationStack(&info);
}
void TestFramework::Init()
{
    std::cout << "Init" << std::endl;
}
