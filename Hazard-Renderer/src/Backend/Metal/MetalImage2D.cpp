
#include "MetalImage2D.h"
#ifdef HZR_INCLUDE_METAL

namespace HazardRenderer::Metal
{
	MetalImage2D::MetalImage2D(Image2DCreateInfo* info)
    {
        HZR_PROFILE_FUNCTION();
        HZR_ASSERT(!info->DebugName.empty(), "Debug name required");
        HZR_ASSERT(info->Format != ImageFormat::None, "Image format cannot be none");
        HZR_ASSERT(info->Usage != ImageUsage::None, "Image usage cannot be none");

        m_DebugName = info->DebugName;
        m_Width = info->Width;
        m_Height = info->Height;
        m_Format = info->Format;
        m_MipLevels = 1; //TODO: Make this work
        m_Usage = info->Usage;
    }
    MetalImage2D::~MetalImage2D()
    {
        
    }
    void MetalImage2D::Invalidate()
    {
        
    }
    void MetalImage2D::Release()
    {
        
    }
    void MetalImage2D::Release_RT()
    {
        
    }
    void MetalImage2D::Resize_RT(uint32_t width, uint32_t height)
    {
        
    }
}
#endif
