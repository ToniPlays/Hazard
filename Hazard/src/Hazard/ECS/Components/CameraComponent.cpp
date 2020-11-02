#pragma once
#include <hzrpch.h>
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"
#include "Hazard/Modules/Renderer/RenderEngine.h"

#include <glm/glm.hpp>
#include "Platform/Rendering/OpenGL/Textures/OpenGLSkybox.h"
#include "Platform/Rendering/OpenGL/Textures/OpenGLCubemapTexture.h"

namespace Hazard {

	CameraComponent::CameraComponent() : Component("Camera Component") 
	{
		
	}
	CameraComponent::~CameraComponent()
	{

	}
	void CameraComponent::OnAttach()
	{
		RecalculateViewMatrix();
		skybox = new OpenGLSkybox();
		std::vector<std::string> paths = std::vector<std::string>(6);
		paths[0] = "res/textures/sea-right.jpg";
		paths[1] = "res/textures/sea-left.jpg";
		paths[2] = "res/textures/sea-top.jpg";
		paths[3] = "res/textures/sea-bottom.jpg";
		paths[4] = "res/textures/sea-front.jpg";
		paths[5] = "res/textures/sea-back.jpg";


		skybox->SetCubemap(new OpenGLCubemapTexture("Skybox", paths));
		skyboxShader = RendererAPI::Create<Shader>("res/shaders/skyboxShader.glsl");

	}
	void CameraComponent::RecalculateViewMatrix()
	{
		PROFILE_FN();
		float aspectX = RenderEngine::GetStats().GetAspectRatio();
		glm::mat4 projection;
		if (type == CameraType::Orthographic) 
			projection = glm::ortho(-aspectX * FovSize, aspectX * FovSize, -FovSize, FovSize, clipping.x, clipping.y);
		else 
			projection = glm::perspective((float)Math::ToRadians(FovSize), aspectX, clipping.x, clipping.y);

		viewProjection = projection * glm::inverse(Transform::AsMat4NoScale(parent->GetComponent<TransformComponent>()->transform));
		
		PROFILE_FN_END();
	}
	void CameraComponent::DrawBackground()
	{
		/*skyboxShader->Bind();

		skyboxShader->SetUniform("viewProjection", viewProjection);
		skyboxShader->SetUniform("offset", parent->GetComponent<TransformComponent>()->transform->position);
		skybox->Render();*/
	}
	void CameraComponent::SerializeComponent(YAML::Emitter& out)
	{
		out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Type" << YAML::Value << (type == CameraType::Perspective ? "Perspective" : "Orthographic");
		out << YAML::Key << "Clipping" << YAML::Value; Serializer::Serialize(out, clipping);
		out << YAML::Key << "Size" << YAML::Value << FovSize;
		out << YAML::Key << "Clear Color" << YAML::Value; Serializer::Serialize(out, clearColor);
		out << YAML::EndMap;
	}
	void CameraComponent::DeserializeComponent(YAML::Node in)
	{
		type = in["Type"].as<std::string>() == "Perspective" ? CameraType::Perspective : CameraType::Orthographic;
		clipping = Deserializer::Deserialize<Vector2<float>>(in["Clipping"]);
		FovSize = in["Size"].as<float>();
		clearColor = Deserializer::Deserialize<Color>(in["Clear Color"]);
	}
	void CameraComponent::PostDeserialize() {
		RecalculateViewMatrix();
	}
}