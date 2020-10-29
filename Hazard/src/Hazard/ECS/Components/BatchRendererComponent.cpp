#pragma once
#include <hzrpch.h>
#include "BatchRendererComponent.h"
#include "Hazard/Modules/Renderer/Renderer2D.h"


namespace Hazard {

	BatchRendererComponent::BatchRendererComponent() : Component("BatchRendererComponent")
	{
		size = 1;
		
	}
	BatchRendererComponent::~BatchRendererComponent()
	{

	}
	void BatchRendererComponent::OnAttach() 
	{
		textureIndex = 4;
	}
	void BatchRendererComponent::OnDetach() 
	{
		
	}
	void BatchRendererComponent::OnRender() {
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++)
				Renderer2D::DrawQuad({ float(y), float(x), 0.0f }, { 0.9f, 0.9f, 0.9f }, 
					{ float(x) / size, 0.8f, float(y) / size, 1.0f }, "res/textures/checker.png");
				//Renderer2D::DrawQuad({ float(y), float(x), 0.0f }, { 0.9f, 0.9f, 0.9f }, 
				//	{ float(x) / size, 0.8f, float(y) / size, 1.0f });
		}
	}
	void BatchRendererComponent::SerializeComponent(YAML::Emitter& out)
	{
		out << YAML::Key << "BatchRendererComponent" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Size" << YAML::Value << size;
		out << YAML::EndMap;
	}
	void BatchRendererComponent::DeserializeComponent(YAML::Node in)
	{
		size = in["Size"].as<float>();
	}
}