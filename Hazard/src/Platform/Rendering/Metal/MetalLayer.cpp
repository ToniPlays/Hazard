#include <hzrpch.h>
#include "MetalLayer.h"
#include "MetalContext.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


namespace Hazard::Rendering::Metal
{
    

    MetalLayer::MetalLayer(GLFWwindow* window, MTL::Device* device)
    {
       
    }
    void MetalLayer::Present(MTL::CommandBuffer* buffer) {
        //buffer->presentDrawable((MTL::Drawable*)layer);
    }
}
