#pragma once
#include <raylib.h>
#include <cmath>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"  


#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


class Simulation {
public:
    Simulation(float width, float height)
        : width {width}
        , height {height} 
    {
        shader = LoadShader(0, TextFormat("../shaders/mandelbrot.fs", GLSL_VERSION));
        iterations = 10;

        r_offset_loc = GetShaderLocation(shader, "r_offset");
        i_offset_loc = GetShaderLocation(shader, "i_offset");
        iterations_loc = GetShaderLocation(shader, "iterations");

        SetShaderValue(shader, r_offset_loc, r_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, i_offset_loc, i_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, iterations_loc, &iterations, SHADER_UNIFORM_INT);

        float slider_value = iterations;
    }

    void handleEvents() {
        // moving around the plane
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse_delta = GetMouseDelta();
            float r_translation = (mouse_delta.x/width)* abs(r_offset[0] - r_offset[1]);
            float i_translation = (mouse_delta.y/height)* abs(i_offset[0] - i_offset[1]);
            // glsl has 0,0 at bottom left 
            r_offset[0] -= r_translation;
            r_offset[1] -= r_translation;
            i_offset[0] += i_translation;
            i_offset[1] += i_translation;
        }
        // zoom
        float wheel_move = GetMouseWheelMove()*100;
        float zoom = (wheel_move/width)* abs(r_offset[0] - r_offset[1]);
        r_offset[0] += zoom;
        r_offset[1] -= zoom;
        i_offset[0] += zoom;
        i_offset[1] -= zoom;
    }

    void updateState() {
        SetShaderValue(shader, r_offset_loc, r_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, i_offset_loc, i_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, iterations_loc, &iterations, SHADER_UNIFORM_INT);
        iterations = slider_value;
    }

    void draw() {
        BeginShaderMode(shader);  // Begin using the shader
        DrawRectangle(0, 0, width, height, WHITE);  // Draw a full screen white rectangle
        EndShaderMode();  // End using the shader

        // should i name them orbits?
        int res = GuiToggle((Rectangle){ 0, 0, 40, 30 }, "Orbits", &toggle_active);
        GuiSliderBar ((Rectangle){ 100, 100, 200, 25 }, "Iterations", NULL, &slider_value, 0, 1000);
    }

private:
    float width;
    float height;
    Shader shader;
    float r_offset[2] = { -2.0f, 1.0f };
    float i_offset[2] = { -1.5f, 1.5f };
    int iterations;

    // shader uniform loc
    int r_offset_loc;
    int i_offset_loc;
    int iterations_loc;

    // gui 
    bool toggle_active = false;
    float slider_value;
};