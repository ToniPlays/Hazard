#type Vertex
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/GridUniform.glsl"

layout(location = 0) out vec3 nearPoint;
layout(location = 1) out vec3 farPoint;

vec3 gridPlane[6] = vec3[](
	vec3(-1,  1, 0), vec3(-1, -1, 0), vec3( 1,  1, 0),
	vec3( 1, -1, 0), vec3( 1,  1, 0), vec3(-1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 proj)
{
	mat4 viewInv = inverse(view);
	mat4 projInv = inverse(proj);

	vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
	return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() 
{
	vec3 p = gridPlane[gl_VertexIndex];

	nearPoint = UnprojectPoint(p.x, p.y, 0.0, u_Camera.View, u_Camera.Projection);
	farPoint = UnprojectPoint(p.x, p.y, 1.0, u_Camera.View, u_Camera.Projection);

	gl_Position = vec4(p, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec3 nearPoint;
layout(location = 1) in vec3 farPoint;

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/GridUniform.glsl"

layout(location = 0) out vec4 OutputColor;

const float axisWidth = 1.25;
const float ZNear = 0.03f;
const float ZFar = 1000.0f;
    
const float nextZoom = 10.0;
const float lastZoom = 1.0;

vec4 EditorGrid(vec3 fragPos3D, float scale, bool drawAxis)
{
    vec2 coord = fragPos3D.xz * (1.0 / scale); // Use the scale variable to set the distance between the lines
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);

    float minimumz = min(derivative.y, 1) * axisWidth;
    float minimumx = min(derivative.x, 1) * axisWidth;
    vec4 color = vec4(0.25, 0.25, 0.25, 1.0 - min(line, 1.0));

    if(!drawAxis) 
        return color;
    // z axis
    if(fragPos3D.x > -axisWidth * minimumx && fragPos3D.x < axisWidth * minimumx) 
    {
        color = vec4(0.0, 0.5, 1.0, 0.8);
    }
    // x axis
    if(fragPos3D.z > -axisWidth * minimumz && fragPos3D.z < axisWidth * minimumz) 
    {
        color = vec4(1.0, 0.0, 0.0, 0.8);
    }

    return color;
}

float ComputeDepth(vec3 pos)
{
    vec4 clipSpacePos = u_Camera.ViewProjection * vec4(pos.xyz, 1.0);
    return (clipSpacePos.z / clipSpacePos.w);
}
float ComputeLinearDepth(vec3 pos)
{
    float near = ZNear;
    float far = ZFar;

    vec4 clipSpacePos = u_Camera.ViewProjection * vec4(pos.xyz, 1.0);
    float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - clipSpaceDepth * (far - near));
    return linearDepth / far;
}

void main() 
{
	float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = ComputeDepth(fragPos3D);
    
    float linearDepth = ComputeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));
	float gridFading = pow((u_Grid.Zoom - u_Grid.Scale * lastZoom) / (u_Grid.Scale * nextZoom - u_Grid.Scale * lastZoom), 2.5);


    vec4 g1 = EditorGrid(fragPos3D, u_Grid.Scale * lastZoom, true);
    vec4 g2 = EditorGrid(fragPos3D, u_Grid.Scale * nextZoom, true);

	OutputColor = mix(g1, g2, clamp(gridFading, 0.0, 1.0)) * float(t > 0.0 && t < 1.0);
    OutputColor.a *= fading;
}