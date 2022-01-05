#pragma once

#include "Hazard/Rendering/Texture.h"

namespace Hazard::Rendering::Metal
{
    class MetalTexture2D : public Texture2D {
    public:
        MetalTexture2D(Texture2DCreateInfo* info) {};
        ~MetalTexture2D() {};

        uint32_t GetWidth() const override { return m_Width; };
        uint32_t GetHeight() const override { return m_Height; };
        virtual void Bind(uint32_t slot = 0) const override {};

        virtual void Resize(uint32_t width, uint32_t height) override {};
        virtual Buffer GetWriteBuffer() override { return Buffer(); };
        virtual bool Loaded() const override { return m_Loaded; };
        virtual ImageFormat GetFormat() const override { return m_Format; }


        virtual float GetAspectRatio() { return (float)m_Width / (float)m_Height; }
        virtual Buffer GetBuffer() const { return m_LocalData; };
        virtual const Buffer& GetBuffer() { return m_LocalData; }

    private:

        Buffer m_LocalData;

        ImageFormat m_Format;
        bool m_Loaded = false;

        int m_Width;
        int m_Height;
    };
}
