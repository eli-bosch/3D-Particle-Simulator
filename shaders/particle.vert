#version 460 core

struct Particle {
    vec4 position;
    vec4 velocity;
};

layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

uniform mat4 view;
uniform mat4 projection;
uniform float fov;
uniform float viewportHeight;

void main() {
    vec4 worldPos = particles[gl_VertexID].position;
    gl_Position = projection * view * worldPos;
    gl_PointSize = max(3.0, fov * viewportHeight / gl_Position.w)/15;
}
