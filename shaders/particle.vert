#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float fov;
uniform float viewportHeight;

void main() {
    vec4 viewPos = view * vec4(aPos, 1.0);
    gl_Position = projection * viewPos;

    float dist = length(viewPos.xyz);
    float pointSize = (1.0 / dist) * viewportHeight / (2.0 * tan(fov / 2.0));
    gl_PointSize = clamp(pointSize, 2.0, 10.0);
}
