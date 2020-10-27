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


		//Projection
		CameraType type = CameraType::Orthographic;
		float FovSize = 10;
		Vector2<float> clipping = { 0, 1000 };
		void SetType(CameraType type) { this->type = type; }

		void RecalculateViewMatrix(float w, float h);

		void SerializeComponent(YAML::Emitter& out) override;
		void DeserializeComponent(YAML::Node in) override;

		CameraType GetType() { return type; }
		glm::mat4 GetViewProjection() { return viewProjection; }
	private:
		glm::mat4 viewProjection;
	};
}