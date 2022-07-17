#type Vertex
#version 450


layout(std140, binding = 0) uniform Camera {
	uniform mat4 u_ViewProjection;
	uniform mat4 u_Projection;
	uniform mat4 u_View;
	uniform vec4 u_Position;
} u_Camera;


layout(std140, binding = 1) uniform Grid {
	uniform float u_ZNear;
	uniform float u_ZFar;
	uniform float u_Scale;
	uniform float u_ScaleFading;

} u_Grid;


layout(location = 0) out vec3 nearPoint;
layout(location = 1) out vec3 farPoint;

vec3 gridPlane[6] = vec3[](
	vec3( 1,  1, 0), vec3(-1, -1, 0), vec3(-1,  1, 0),
	vec3(-1, -1, 0), vec3( 1,  1, 0), vec3( 1, -1, 0)
);

vec3 Unproject(vec2 pos, float z, mat4 view, mat4 projection) {
	
	mat4 invView = inverse(view);
	mat4 invProj = inverse(projection);
	vec4 unprojectedPoint = invView * invProj * vec4(pos, z, 1.0);
	return unprojectedPoint.xyz / unprojectedPoint.w;
}


void main() 
{
	vec3 pos = gridPlane[gl_VertexIndex];
	nearPoint = Unproject(pos.xy, 0.0, u_Camera.u_View, u_Camera.u_Projection).xyz;
	farPoint = Unproject(pos.xy, 1.0, u_Camera.u_View, u_Camera.u_Projection).xyz;

	gl_Position = vec4(pos, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec3 nearPoint;
layout(location = 1) in vec3 farPoint;fsd<

layout(std140, binding = 0) uniform Camera {
	uniform mat4 u_ViewProjection;
	uniform mat4 u_Projection;
	uniform mat4 u_View;
	uniform vec4 u_Position;
} u_Camera;


layout(std140, binding = 1) uniform Grid {
	uniform float u_ZNear;
	uniform float u_ZFar;
	uniform float u_Scale;
	uniform float u_ScaleFading;

} u_Grid;

layout(location = 0) out vec4 color;

const float axisWidth = 2.0;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    if(!drawAxis) return color;
    // z axis
    if(fragPos3D.x > -axisWidth * minimumx && fragPos3D.x < axisWidth * minimumx) {
        color = vec4(0.0, 0.5, 1.0, 0.8);
    }
    // x axis
    if(fragPos3D.z > -axisWidth * minimumz && fragPos3D.z < axisWidth * minimumz) {
        color = vec4(1.0, 0.0, 0.0, 0.8);
    }
    return color;
}
float computeDepth(vec3 pos) 
{
    vec4 clip_space_pos = u_Camera.u_Projection * u_Camera.u_View * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}
float computeLinearDepth(vec3 pos) 
{
    vec4 clip_space_pos = u_Camera.u_Projection * u_Camera.u_View * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * u_Grid.u_ZNear * u_Grid.u_ZFar) / (u_Grid.u_ZFar + u_Grid.u_ZNear - clip_space_depth * (u_Grid.u_ZFar - u_Grid.u_ZNear)); // get linear value between 0.01 and 100
    return linearDepth / u_Grid.u_ZFar - 0.002; // normalize
}
void main() 
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    color = (grid(fragPos3D, u_Grid.u_Scale, true)) * float(t > 0); // adding multiple resolution for the grid
    color.a *= fading;
}