#pragma once
#include <hzrpch.h>
#include "RenderCommand.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "Hazard/Rendering/2D/Renderer2D.h"

namespace Hazard::Rendering 
{
    void RenderCommand::DrawQuad(const ECS::SpriteRendererComponent& comp, const ECS::TransformComponent& transform)
    {
        Quad quad;
        quad.Transform = Math::ToTransformMatrix(transform.m_Translation, transform.m_Rotation, transform.m_Scale);
        quad.Color = comp.m_Tint;
        quad.Texture = comp.m_Texture;
        s_Engine->Get2D().Submit(quad);
    }
    void RenderCommand::ExecuteCallbacks()
    {
        for (auto& callback : m_OnRender) 
            callback();
    }
}
