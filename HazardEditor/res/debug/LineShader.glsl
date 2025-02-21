#type Fragment
#version 450


			layout(location = 0) in vec4 IN_Color;
			

			layout(location = 0) out vec4 OUT_Color;
			layout(location = 1) out uint OUT_EntityID;
			

			void main() 
			{
				OUT_Color = IN_Color;
				OUT_EntityID = 0;
			}

#type Vertex
#version 450
#include "../Uniforms/CameraUniform.glslh"


			layout(location = 0) in vec3 IN_Position;
			layout(location = 1) in vec4 IN_Color;
			

			layout(location = 0) out vec4 OUT_Color;
			

			void main() 
			{
				OUT_Color = IN_Color;
				gl_Position = u_Camera.ViewProjection * vec4(IN_Position, 1.0);
			}

