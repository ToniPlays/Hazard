#pragma once
#include "Component.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"


namespace Hazard {
	enum class CameraType {
		Orthographic, Perspective
	};

	class HAZARD_API CameraComponent : public Component {
	public:
		CameraComponent();
		~CameraComponent();

		void OnAttach() override;

		CameraType type = CameraType::Orthographic;
		float FovSize = 10;
		Vector2<float> clipping = { 0, 1000 };
		Color clearColor = Color();

		//Projection

		void SetType(CameraType type) {
			this->type = type;
			RecalculateViewMatrix();
		}
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