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
		vertexArray = RendererAPI::Create<VertexArray>();


		VertexBuffer* buffer = RendererAPI::VertexBuffer(ShaderDataType::Float3, "position");
		float vertices[12] = {
			-0.5, -0.5, 0.0,
			 0.5, -0.5, 0.0,
			 0.5,  0.5, 0.0,
			-0.5,  0.5, 0.0
		};
		buffer->SetData(vertices, sizeof(vertices) / sizeof(float));

		VertexBuffer* colorBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float4, "color");

		vertexArray->SetLayout({ buffer, colorBuffer });

		int indices[6] = { 0, 1, 2, 2, 3, 0 };
		IndexBuffer* indexBuffer = RendererAPI::Create<IndexBuffer>();
		indexBuffer->SetData(indices, sizeof(indices));
		vertexArray->SetIndexBuffer(indexBuffer);

		SetTint(Color::FromHex("#FFFFFF"));
	}

	SpriteRenderer::~SpriteRenderer()
	{
		delete vertexArray;

	}

	void SpriteRenderer::OnRender() {
		RenderEngine::Draw(vertexArray);
	}
	void SpriteRenderer::SetTint(Color _tint)
	{
		this->tint = _tint;
		VertexBuffer* positions = vertexArray->GetBuffer("position");
		uint32_t size = positions->GetLength() / ComponentCount(positions->GetType());

		float color[4] = { tint.r, tint.g, tint.b, tint.a };
		std::vector<float> newColor;

		for (uint32_t i = 0; i < size * sizeof(color) / sizeof(float); i++)
			newColor.push_back(color[i % 4]);

		VertexBuffer* colorBuffer = vertexArray->GetBuffer("color");
		colorBuffer->SetData(newColor.data(), newColor.size());
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