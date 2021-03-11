
#include "Hazard/Math/Color.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
Camera component and post processing effects
*/
namespace Hazard::ECS 
{
	//Camera projection types
	enum Projection { Perspective = 0, Orthographic };

	struct CameraComponent {

		Projection type = Projection::Perspective;
		float fov = 10.0f;
		Color bgColor;

		glm::mat4 projection;
		float width, height;

		//Get and set camera projection type
		int GetProjectionType() { return type; }
		void SetProjection(Projection t) { type = t; RecalculateProjection(width, height); }

		//Recalculate camera projection
		void RecalculateProjection(float w, float h) {

			width = w;
			height = h;

			float aspectRatio = w / h;

			if (type == Perspective) {
				//Calculate perspective projection
				projection = glm::perspective(glm::radians(fov), aspectRatio, 0.01f, 1000.0f);
			}
			else
			{
				//Calculate orthographic projection
				float orthoLeft = -fov * aspectRatio * 0.5f;
				float orthoRight = fov * aspectRatio * 0.5f;
				float orthoBottom = -fov * 0.5f;
				float orthoTop = fov * 0.5f;

				projection = glm::ortho(orthoLeft, orthoRight,
					orthoBottom, orthoTop, -1000.0f, 1000.0f);
			}
		}
	};
}