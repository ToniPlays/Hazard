#pragma once
#include <hzrpch.h>
#include "BatchRendererComponent.h"
#include "Hazard/Modules/Renderer/Renderer2D.h"
#include "TransformComponent.h"


namespace Hazard {

	BatchRendererComponent::BatchRendererComponent() : Component("BatchRendererComponent")
	{
	}
	BatchRendererComponent::~BatchRendererComponent()
	{

	}
	void BatchRendererComponent::OnAttach() 
	{

	}
	void BatchRendererComponent::OnDetach() 
	{
		
	}
	void BatchRendererComponent::OnRender() {
		Transform* transform = parent->GetComponent<Transform>();
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				Renderer2D::DrawQuad(Transform({ float(x), float(y), 0.0f }, Vector3<float>::zero, { 0.8f, 0.8f, 1.0f }), { float(x) / size, 0.8f, float(y) / size, 1.0f }, "res/textures/checker.png");
				//Renderer2D::DrawQuad((Matrix4)*transform, { float(x) / size, 0.8f, float(y) / size, 1.0f });
			}
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