#pragma once
#include <raylib.h>
#include <vector>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

class Simulation {
public:
    Simulation(float width, float height, int fps)
        : width {width}
        , height {height}
        , fps {fps}
        , iterations {250}
        , slider_value{(float)iterations}
        , show_orbits {false}
    {
        shader = LoadShader(0, TextFormat("../shaders/mandelbrot.fs", GLSL_VERSION));

        r_offset_loc = GetShaderLocation(shader, "r_offset");
        i_offset_loc = GetShaderLocation(shader, "i_offset");
        iterations_loc = GetShaderLocation(shader, "iterations");

        SetShaderValue(shader, r_offset_loc, r_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, i_offset_loc, i_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, iterations_loc, &iterations, SHADER_UNIFORM_INT);
    }

    void handleEvents();
    void updateState();
    void draw();

private:

    // get vector of orbits from point c
    std::vector<Vector2> get_orbits();
    void draw_orbits();
    void drawAxis();
    
    float width;
    float height;
    int fps;
    Shader shader;
    float r_offset[2] = { -2.0f, 1.0f };
    float i_offset[2] = { -1.5f, 1.5f };
    int iterations;

    // shader uniform loc
    int r_offset_loc;
    int i_offset_loc;
    int iterations_loc;

    // gui 
    bool show_orbits;
    float slider_value;
};