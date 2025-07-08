#version 460 core
out vec4 FragColor;

uniform vec3 color; // Optional, can hardcode color instead

void main() {
    float dist = length(gl_PointCoord - vec2(0.5)); // Distance from center of point
    float alpha = smoothstep(0.5, 0.45, dist);      // Soft circular edge

    if (alpha <= 0.01)
        discard;  // Fully transparent outside circle

    FragColor = vec4(color, alpha); // Transparent edges
}
