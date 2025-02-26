
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <iostream>
#include "simulation.hpp" 

int main() {
    const float WINDOW_WIDTH = 1000.0f;
    const float WINDOW_HEIGHT = 1000.0f;
    int FPS = 60;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Mandelbrot set");
    Simulation sim(WINDOW_WIDTH, WINDOW_HEIGHT);

    SetTargetFPS(FPS);
    // Simulation Loop
    while (!WindowShouldClose()) {
        // 1. Event Handling
        sim.handleEvents();

        // 2. Updating State
        sim.updateState();

        // 3. Drawing Objects
        BeginDrawing();
	        ClearBackground(WHITE);
            sim.draw();
            DrawText(TextFormat("Target FPS: %i \nCurrent FPS: %i", FPS, GetFPS()), 20, 20, 30, BLUE);

        EndDrawing();
    }    

    CloseWindow();
}
