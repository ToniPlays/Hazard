#pragma once
#include "Component.h"



namespace Hazard {
	enum class CameraType {
		Orthographic, Perspective
	};

	class HAZARD_API CameraComponent : public Component {
	public:
		CameraComponent();
		~CameraComponent();

		CameraType type = CameraType::Orthographic;
		float FovSize = 10;
		Vector2<float> clipping = { 0, 1000 };
		Color clearColor = Color();

		//Projection

		void SetType(CameraType type) { this->type = type; }
		void RecalculateViewMatrix();
		
		void SerializeComponent(YAML::Emitter& out) override;
		void DeserializeComponent(YAML::Node in) override;
		void PostDeserialize() override;

		CameraType GetType() { return type; }
		glm::mat4 GetViewProjection() { return viewProjection; }
	private:
		glm::mat4 viewProjection;
	};
}