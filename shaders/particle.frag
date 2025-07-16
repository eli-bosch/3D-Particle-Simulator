#version 460 core

out vec4 FragColor;

void main() {
    // gl_PointCoord ranges from (0,0) to (1,1) within the point
    vec2 coord = gl_PointCoord * 2.0 - 1.0;  // Now from (-1, -1) to (1, 1)
    float dist = dot(coord, coord);         // Squared distance from center

    if (dist > 1.0)
        discard;  // Outside the unit circle — discard

    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White circle
}
