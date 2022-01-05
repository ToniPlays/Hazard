#pragma once

namespace CA
{
    class MetalLayer;
}

namespace Hazard::Rendering::Metal
{
    class MetalLayer {
    public:
        MetalLayer();
        ~MetalLayer() = default;
    private:
        CA::MetalLayer* m_Layer;
    };
}
