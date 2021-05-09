#pragma once
#include <hzrpch.h>
#include "OpenGLEnvironmentMap.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"
#include "Hazard/Rendering/RenderCommand.h"

#include <stb_image.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Hazard::Rendering::OpenGL {

    OpenGLEnvironmentMap::OpenGLEnvironmentMap(const char* file, TextureSpecs& specs) : EnvinronmentMap(file)
    {
        m_Spec = specs;

        TextureSpecs textSpec;
        textSpec.dataType = TextureDataType::HDR;
        m_RawTexture = RenderUtils::Create<Texture2D>(file, textSpec);

        TextureSpecs cubeSpecs;
        cubeSpecs.width = m_Spec.width;
        cubeSpecs.height = m_Spec.height;
        cubeSpecs.dataType = TextureDataType::HDR;

        m_Cubemap = RenderUtils::Create<CubemapTexture>(cubeSpecs);
        GenerateCubemap();
    }
    OpenGLEnvironmentMap::~OpenGLEnvironmentMap()
    {

    }
    void OpenGLEnvironmentMap::SetTexture(int side, const std::string& file)
    {

    }
    void OpenGLEnvironmentMap::SetIrradiance(float irradiance)
    {

    }
    void OpenGLEnvironmentMap::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_Cubemap->GetID());
    }
    void OpenGLEnvironmentMap::Unbind(uint32_t slot) const
    {
        glBindTextureUnit(slot, 0);
    }
    void OpenGLEnvironmentMap::GenerateCubemap()
    {
        float vertices[24] = {
            // positions
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f
        };
        uint32_t indices[36] = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };

        HZR_CORE_WARN("Generating cubemap from .hrd file");

        VertexArray* cubeArray = RenderUtils::Create<VertexArray>();
        VertexBuffer* positions = RenderUtils::Create<VertexBuffer>((uint32_t)sizeof(vertices));

        positions->SetLayout({ { ShaderDataType::Float3, "pos" } });

        positions->SetData(vertices, 24 * sizeof(float));
        cubeArray->AddBuffer(positions);

        IndexBuffer* indexBuffer = RenderUtils::Create<IndexBuffer>();
        indexBuffer->SetData(indices, 36);
        cubeArray->SetIndexBuffer(indexBuffer);

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        Shader* convertShader = RenderUtils::Create<Shader>("res/shaders/equirectangularToCube.glsl");

        unsigned int captureFBO;
        unsigned int captureRBO;

        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Spec.width, m_Spec.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        glViewport(0, 0, m_Spec.width, m_Spec.height);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

        cubeArray->Bind();
        m_RawTexture->Bind();

        convertShader->Bind();
        convertShader->SetUniformInt("equirectangularMap", 0);
        convertShader->SetUniformMat4("projection", captureProjection);

        for (uint8_t i = 0; i < 6; ++i) 
        {
            convertShader->SetUniformMat4("view", captureViews[i]);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_Cubemap->GetID(), 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            RenderCommand::DrawIndexed(cubeArray, indexBuffer->GetCount());
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        delete cubeArray;
    }

    void OpenGLEnvironmentMap::GenerateIrradiance()
    {

    }
    void OpenGLEnvironmentMap::GeneratePreFilter()
    {

    }
}