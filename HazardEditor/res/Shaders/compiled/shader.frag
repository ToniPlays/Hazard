#version 450

layout(location = 0) in vec4 fragColor;

layout(push_constant) uniform Push {
    mat4 transform;
    mat4 viewProjection;
} push;


layout(location = 0) out vec4 outColor;

void main() {
    outColor = mix(fragColor, fragColor, 0.2f);
}