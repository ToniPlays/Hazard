#version 450

layout(location = 0) in vec2 v_Position;
layout(location = 1) in vec4 v_Color;

layout(push_constant) uniform Push {
    mat4 transform;
    mat4 viewProjection;
} push;


layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = push.viewProjection * push.transform * vec4(v_Position, 0.0, 1.0);
    fragColor = v_Color;
}