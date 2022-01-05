#pragma once



struct GLFWwindow;

namespace MTL
{
class Device;
class CommandBuffer;
}

namespace Hazard::Rendering::Metal
{
    class MetalLayer {
    public:
        MetalLayer(GLFWwindow* window, MTL::Device* device);
        ~MetalLayer() = default;
        
        void Present(MTL::CommandBuffer* buffer);
        
    private:
        
    };
}
