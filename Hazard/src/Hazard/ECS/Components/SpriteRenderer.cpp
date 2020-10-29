#pragma once

#include <hzrpch.h>
#include "SpriteRenderer.h"
#include "Hazard/Modules/Renderer/RendererAPI.h"
#include "Hazard/Modules/Renderer/RenderEngine.h"
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"


namespace Hazard {

	SpriteRenderer::SpriteRenderer() : Component("SpriteRenderer")
	{
		SetTint(Color::FromHex("#FFFFFF"));
	}

	SpriteRenderer::~SpriteRenderer()
	{
		delete vertexArray;
	}

	void SpriteRenderer::OnAttach()
	{
		textureIndex = 3;
	}

	void SpriteRenderer::OnRender() {
		Renderer2D::DrawQuad(Vector3<float>::zero, Vector3<float>::one, tint, "res/textures/hazard.png");
		//Renderer2D::DrawQuad(Vector3<float>::zero, Vector3<float>::one, tint);
	}
	void SpriteRenderer::SetTint(Color _tint)
	{
		this->tint = _tint;
	}
	void SpriteRenderer::SerializeComponent(YAML::Emitter& out)
	{
		out << YAML::Key << "SpriteRenderer" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Tint" << YAML::Value; Serializer::Serialize(out, tint);
		out << YAML::EndMap;
	}
	void SpriteRenderer::DeserializeComponent(YAML::Node in)
	{
		SetTint(Deserializer::Deserialize<Color>(in["Tint"]));
	}
}