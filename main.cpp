#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <iostream>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


Vector2 r_axis{-2.0f, 1.0f};
Vector2 i_axis{-1.5f, 1.5f};
float zoom_factor = 0.01f;

int main() {
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 800;
    int FPS = 30;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Mandelbrot set");
    
    Shader shader = LoadShader(0, TextFormat("../shaders/mandelbrot.fs", GLSL_VERSION));
    float r_offset[2] = { -2.0f, 1.0f };
    float i_offset[2] = { -1.5f, 1.5f };

    int r_offset_loc = GetShaderLocation(shader, "r_offset");
    int i_offset_loc = GetShaderLocation(shader, "i_offset");

    SetShaderValue(shader, r_offset_loc, r_offset, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, i_offset_loc, i_offset, SHADER_UNIFORM_VEC2);

    SetTargetFPS(FPS);


    // Simulation Loop
    while (!WindowShouldClose()) {
        // 1. Event Handling
        if (IsKeyDown(KEY_PAGE_DOWN)) {
            r_offset[0] -= zoom_factor;
            r_offset[1] += zoom_factor;
            i_offset[0] -= zoom_factor;
            i_offset[1] += zoom_factor;
        }
        if (IsKeyDown(KEY_PAGE_UP)) {
            r_offset[0] += zoom_factor;
            r_offset[1] -= zoom_factor;
            i_offset[0] += zoom_factor;
            i_offset[1] -= zoom_factor;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            r_offset[0] += zoom_factor;
            r_offset[1] += zoom_factor;
        }
        if (IsKeyDown(KEY_LEFT)) {
            r_offset[0] -= zoom_factor;
            r_offset[1] -= zoom_factor;
        }
        // glsl has 0,0 at bottom left 
        if (IsKeyDown(KEY_UP)) {
            i_offset[0] += zoom_factor;
            i_offset[1] += zoom_factor;
        }
        if (IsKeyDown(KEY_DOWN)) {
            i_offset[0] -= zoom_factor;
            i_offset[1] -= zoom_factor;
        }

        SetShaderValue(shader, r_offset_loc, r_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, i_offset_loc, i_offset, SHADER_UNIFORM_VEC2);

        // 2. Updating State
        
        // 3. Drawing Objects
        BeginDrawing();
	        ClearBackground(WHITE);

            BeginShaderMode(shader);  // Begin using the shader
            DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WHITE);  // Draw a full screen white rectangle
            EndShaderMode();  // End using the shader
            DrawText(TextFormat("Target FPS: %i \nCurrent FPS: %i", FPS, GetFPS()), 20, 20, 30, RED);
        EndDrawing();
    }    

    CloseWindow();
}
