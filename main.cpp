#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <iostream>

// use GLSL shader!!!
Vector2 r_axis{-2.0f, 1.0f};
Vector2 i_axis{-1.5f, 1.5f};
const float stability_threshold = 2.0f;
const float zoom_factor = 0.01f;


bool isStable(Vector2& C) {
    Vector2 zn{0.0f, 0.0f};
    Vector2 zn_1{0.0f, 0.0f};
    const int iterations = 20;
    
    // can optimize this if zn is smaller then 2 get out of cycle prematurely
    for (int i=0; i<iterations; i++) {
        // zn+1 = zn^2 + C
        // (a+bi)(a+bi) = (a^2-b^2) + (2ab)i
        Vector2 square_zn{
            zn.x*zn.x - zn.y*zn.y,
            2.0f*zn.x*zn.y
        };
        zn_1 = Vector2Add(square_zn, C);
        zn = zn_1;
        if (Vector2Length(zn) > stability_threshold) {
            return false;
        }
    }
    if (Vector2Length(zn) > stability_threshold) {
        return false;
    } 
    return true;
}

void drawMandelbrot(int width, int height) {
    for (int x=0; x<width; x++) {
        for (int y=0; y<height; y++) {

            Vector2 C{
                ((x*1.0f / width) * abs(r_axis.x - r_axis.y)) + r_axis.x,
                ((y*1.0f / height) * abs(i_axis.x - i_axis.y)) + i_axis.x
            };
            if (isStable(C)) {
                DrawPixel(x, y, BLACK);
            } else {
                DrawPixel(x, y, WHITE);
            }
        }
    }
}

int main() {
    Color GREY = {29, 29, 29, 255};

    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 800;
    int FPS = 30;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Mandelbrot set");
    SetTargetFPS(FPS);
    // Simulation Loop
    while (!WindowShouldClose()) {
        // 1. Event Handling
        if (IsKeyDown(KEY_PAGE_DOWN)) {
            r_axis.x *= (1-zoom_factor);
            r_axis.y *= (1-zoom_factor);
            i_axis.x *= (1-zoom_factor);
            i_axis.y *= (1-zoom_factor);

        } else if (IsKeyDown(KEY_PAGE_UP)) {
            r_axis.x *= (1+zoom_factor);
            r_axis.y *= (1+zoom_factor);
            i_axis.x *= (1+zoom_factor);
            i_axis.y *= (1+zoom_factor);
        } else if (IsKeyDown(KEY_RIGHT)) {
            r_axis.x += zoom_factor;
            r_axis.y += zoom_factor;
        } else if (IsKeyDown(KEY_LEFT)) {
            r_axis.x -= zoom_factor;
            r_axis.y -= zoom_factor;
        } else if (IsKeyDown(KEY_UP)) {
            i_axis.x -= zoom_factor;
            i_axis.y -= zoom_factor;
        }  else if (IsKeyDown(KEY_DOWN)) {
            i_axis.x += zoom_factor;
            i_axis.y += zoom_factor;
        } 

        // 2. Updating State
        
        // 3. Drawing Objects
        BeginDrawing();
	    ClearBackground(GREY);
        drawMandelbrot(WINDOW_WIDTH, WINDOW_HEIGHT);
        DrawText(TextFormat("Target FPS: %i \nCurrent FPS: %i", FPS, GetFPS()), 20, 20, 30, RED);
        EndDrawing();
    }    

    CloseWindow();
}
