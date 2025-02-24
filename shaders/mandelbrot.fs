#version 330 core

// check
// https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_julia_set.c
// https://github.com/raysan5/raylib/blob/master/examples/shaders/resources/shaders/glsl330/julia_set.fs
// or
// https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_multi_sample2d.c
// https://github.com/raysan5/raylib/blob/master/examples/shaders/resources/shaders/glsl330/color_mix.fs

uniform vec2 r_offset;
uniform vec2 i_offset;

out vec4 finalColor;

void main() {
    // Get the current fragment's coordinates
    vec2 pix_coords = gl_FragCoord.xy;
    vec2 zn = vec2(0.0, 0.0);
    vec2 zn_1 = vec2(0.0, 0.0);
    int iterations = 20;
    bool is_out = false;

    float WINDOW_WIDTH = 800.0;
    float WINDOW_HEIGHT = 800.0;

    // normalize for plane described by r_offset for real part and i_offset for imaginary part
    vec2 c = vec2(((pix_coords.x / WINDOW_WIDTH)*abs(r_offset.x - r_offset.y))+r_offset.x,
                  ((pix_coords.y / WINDOW_HEIGHT)*abs(i_offset.x - i_offset.y))+i_offset.x);

    float iterations_out = 0.0;
    for (int i=0; i<iterations; i++) {
        // we want to compute zn+1 = zn^2 + C
        zn_1 = vec2(zn.x*zn.x - zn.y*zn.y, 2*zn.x*zn.y) + c;
        zn = zn_1;
        if (length(zn) > 2) {
            is_out = true;
            iterations_out = i;
            break;
        }
    }

    if (is_out) {
        finalColor = vec4(1.0/iterations_out, 1.0/iterations_out, 1.0/iterations_out, 1.0);
    } else {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}