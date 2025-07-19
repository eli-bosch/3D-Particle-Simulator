#version 460 core

struct Particle {
    vec4 position;
    vec4 velocity;
    float radius;
    float padding[3];
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
    
    gl_PointSize = 12.5 / gl_Position.w;
}
