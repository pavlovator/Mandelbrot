#version 330 core

// check
// https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_julia_set.c
// https://github.com/raysan5/raylib/blob/master/examples/shaders/resources/shaders/glsl330/julia_set.fs
// or
// https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_multi_sample2d.c
// https://github.com/raysan5/raylib/blob/master/examples/shaders/resources/shaders/glsl330/color_mix.fs

uniform vec2 r_offset;
uniform vec2 i_offset;
uniform int iterations;

out vec4 finalColor;

// copied from https://gist.github.com/Aeldrion/48c82912f632eec4c8b9da7394b89c5d
vec3 hsv_to_rgb(vec3 color) {
    // Translates HSV color to RGB color
    // H: 0.0 - 360.0, S: 0.0 - 100.0, V: 0.0 - 100.0
    // R, G, B: 0.0 - 1.0

    float hue = color.x;
    float saturation = color.y;
    float value = color.z;

    float c = (value/100) * (saturation/100);
    float x = c * (1 - abs(mod(hue/60, 2) - 1));
    float m = (value/100) - c;

    float r = 0;
    float g = 0;
    float b = 0;
    
    if (hue >= 0 && hue < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (hue >= 60 && hue < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (hue >= 120 && hue < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (hue >= 180 && hue < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (hue >= 240 && hue < 300) {
        r = x;
        g = 0;
        b = c;
    } else if (hue >= 300 && hue < 360) {
        r = c;
        g = 0;
        b = x;
    }

    r += m;
    g += m;
    b += m;

    return vec3(r, g, b);
}

// TODO: reduce error somehow
void main() {
    // Get the current fragment's coordinates
    vec2 pix_coords = gl_FragCoord.xy;
    vec2 zn = vec2(0.0, 0.0);
    vec2 zn_1 = vec2(0.0, 0.0);
    bool is_out = false;

    float WINDOW_WIDTH = 1000.0;
    float WINDOW_HEIGHT = 1000.0;

    // normalize for plane described by r_offset for real part and i_offset for imaginary part
    vec2 c = vec2(((pix_coords.x / WINDOW_WIDTH)*abs(r_offset.x - r_offset.y))+r_offset.x,
                  ((pix_coords.y / WINDOW_HEIGHT)*abs(i_offset.x - i_offset.y))+i_offset.x);

    float iterations_out = 0.0;
    for (int i=0; i<iterations; i++) {
        // we want to compute zn+1 = zn^2 + C
        zn_1 = vec2(zn.x*zn.x - zn.y*zn.y, 2.0f*zn.x*zn.y) + c;
        zn = zn_1;
        if (length(zn) > 2) {
            is_out = true;
            iterations_out = i;
            break;
        }
    }

    if (is_out) {
        vec3 hsv = vec3((iterations_out/iterations)*360.0, 100.0, 100.0);
        vec3 rgb = hsv_to_rgb(hsv);
        finalColor = vec4(rgb.r, rgb.g, rgb.b, 1.0);
    } else {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}