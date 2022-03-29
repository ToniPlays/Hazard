
#include "RenderCommand.h"
#include "Backend/Core/Window.h"

namespace HazardRenderer 
{
    void RenderCommand::BeginWorld(WorldSettings settings)
    {
        s_DrawList.Settings = settings;
    }
    //void RenderCommand::DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, glm::vec4 color)
    //{
    //    DrawQuad(pipeline, transform, color);
    //}
    //void RenderCommand::DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, Ref<Texture2D> texture)
    //{
    //   DrawQuad(pipeline, transform, glm::vec4(1.0f), texture);
    //}
    void RenderCommand::DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, glm::vec4 color)
    {
        Ref<RawMesh> quad = nullptr;
        InstancedMesh mesh = {};
        mesh.Transforms.push_back(transform);

        PipelineRenderable renderable = {};// ((s_DrawList.Geometry[pipeline];
    }
    void RenderCommand::Flush()
    {
        __debugbreak();
        //s_DrawList.Geometry.clear();
    }
    void RenderCommand::Init(Window* window)
    {
        s_Context = window->GetContext();
    }
}