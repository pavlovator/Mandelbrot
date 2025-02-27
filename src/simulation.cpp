#include "simulation.hpp"
#include <cmath>

#include <iostream>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"  


void Simulation::handleEvents() {

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

void Simulation::updateState() {
    iterations = slider_value;
    SetShaderValue(shader, r_offset_loc, r_offset, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, i_offset_loc, i_offset, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, iterations_loc, &iterations, SHADER_UNIFORM_INT);
}

void Simulation::draw() {
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
}

std::vector<Vector2> Simulation::get_orbits() {
    std::vector<Vector2> orbits;
    // compute what is this normalized ...
    Vector2 pix_coords = GetMousePosition();
    // normalize
    Vector2 c {
        ((pix_coords.x / width)*abs(r_offset[0] - r_offset[1]))+r_offset[0],
        ((1.0f - pix_coords.y / height)*abs(i_offset[0] - i_offset[1]))+i_offset[0]
    };

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

void Simulation::draw_orbits() {
    //chceme sedost ... ako indikator poradia
    std::vector<Vector2> orbits = get_orbits();
    // draw dots
    for (const Vector2& orb: orbits) {
        //std::cout << orb.x << " : " << orb.y << std::endl;
        DrawCircleGradient(orb.x, orb.y, 5, {0, 0, 0, 255}, {255, 255, 255, 150});
    }
    for (int i=1; i<orbits.size(); ++i) {
        DrawLine(orbits[i-1].x, orbits[i-1].y, orbits[i].x, orbits[i].y, WHITE);
    }
}

void Simulation::drawAxis() {
    float w_2 = width / 2.0f;
    float h_2 = height / 2.0f;

    // draw r-axis
    DrawLine(0, h_2, width, h_2, WHITE);
    // draw i-axis
    DrawLine(w_2, 0, w_2, height, WHITE);

}