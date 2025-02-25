#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <iostream>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


int main() {
    const float WINDOW_WIDTH = 1000.0f;
    const float WINDOW_HEIGHT = 1000.0f;
    int FPS = 60;
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

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse_delta = GetMouseDelta();
            float r_translation = (mouse_delta.x/WINDOW_WIDTH)* abs(r_offset[0] - r_offset[1]);
            float i_translation = (mouse_delta.y/WINDOW_HEIGHT)* abs(i_offset[0] - i_offset[1]);
            // glsl has 0,0 at bottom left 
            r_offset[0] -= r_translation;
            r_offset[1] -= r_translation;
            i_offset[0] += i_translation;
            i_offset[1] += i_translation;
        }
        // 5 to move it faster ... probably make some sort of momentum
        float wheel_move = GetMouseWheelMove()*100;
        float zoom = (wheel_move/WINDOW_WIDTH)* abs(r_offset[0] - r_offset[1]);
        r_offset[0] += zoom;
        r_offset[1] -= zoom;
        i_offset[0] += zoom;
        i_offset[1] -= zoom;



        // 2. Updating State
        SetShaderValue(shader, r_offset_loc, r_offset, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, i_offset_loc, i_offset, SHADER_UNIFORM_VEC2);
        
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
