#pragma once
#include <raylib.h>
#include <cmath>

#include <iostream>
#include <vector>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"  


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
        , iterations {100}
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

    void handleEvents() {

        int result = GuiSliderBar((Rectangle){ 10, 920, 150, 20 }, "", NULL, &slider_value, 0, 1000);

        if (result != 0) {
           return; 
        }
        
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
        iterations = slider_value;
        SetShaderValue(shader, r_offset_loc, r_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, i_offset_loc, i_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, iterations_loc, &iterations, SHADER_UNIFORM_INT);
    }

    void draw() {
        BeginShaderMode(shader);  // Begin using the shader
        DrawRectangle(0, 0, width, height, WHITE);  // Draw a full screen white rectangle
        EndShaderMode();  // End using the shader

        // gui components
        // toto som dost ojebal ale nechce sa mi to uz viac riesit
        GuiSliderBar((Rectangle){ 10, 920, 150, 20 }, "", NULL, &slider_value, 0, 1000);
        DrawText(TextFormat("Iter: %i ", iterations), 170, 920, 15, WHITE);
        DrawText(TextFormat("Target FPS: %i \nCurrent FPS: %i", fps, GetFPS()), 10, 950, 20, WHITE);
        GuiToggle((Rectangle){ 0, 0, 40, 30 }, "Orbits", &show_orbits);
        drawAxis();
        if (show_orbits) {
            draw_orbits();
        }

        // should i name them orbits?
    
    }


    // get vector of orbits from point c
    std::vector<Vector2> get_orbits() {
        std::vector<Vector2> orbits;
        // compute what is this normalized ...
        Vector2 pix_coords = GetMousePosition();
        // normalize
        Vector2 c {
            ((pix_coords.x / width)*abs(r_offset[0] - r_offset[1]))+r_offset[0],
            ((1.0f - pix_coords.y / height)*abs(i_offset[0] - i_offset[1]))+i_offset[0]
        };
        std::cout << "---" << std::endl;
        std::cout << c.x << " c " << c.y << std::endl;    
        std::cout << pix_coords.x << " pix_coords " << pix_coords.y << std::endl;        
    
        std::cout << abs(r_offset[0] - r_offset[1]) << std::endl;
        std::cout << abs(i_offset[0] - i_offset[1]) << std::endl;
        std::cout << "---" << std::endl;

        Vector2 zn {0.0f, 0.0f};
        Vector2 zn_1 {0.0f, 0.0f};

        for (int i=0; i<iterations; ++i) {
            zn_1 = Vector2 {
                zn.x*zn.x - zn.y*zn.y + c.x,
                2.0f*zn.x*zn.y + c.y
            };
            zn = zn_1;

            // unnormalize
            Vector2 orb {
                ((zn.x - r_offset[0]) / abs(r_offset[0] - r_offset[1]))*width,
                (1.0f -(zn.y - i_offset[0]) / abs(i_offset[0] - i_offset[1]))*height
            };
            orbits.push_back(orb);
        }

        return orbits;
    }

    void draw_orbits() {
        //chceme sedost ... ako indikator poradia
        std::vector<Vector2> orbits = get_orbits();
        // draw dots
        for (const Vector2& orb: orbits) {
            //std::cout << orb.x << " : " << orb.y << std::endl;
            DrawCircleGradient(orb.x, orb.y, 5, {0, 0, 0, 255}, {200, 200, 200, 255});
        }
        for (int i=1; i<orbits.size(); ++i) {
            DrawLine(orbits[i-1].x, orbits[i-1].y, orbits[i].x, orbits[i].y, WHITE);
        }
    }

    void drawAxis() {
        float w_2 = width / 2.0f;
        float h_2 = height / 2.0f;

        // draw r-axis
        DrawLine(0, h_2, width, h_2, WHITE);
        // draw i-axis
        DrawLine(w_2, 0, w_2, height, WHITE);

    }

private:
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