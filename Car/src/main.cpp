#include <raylib.h>

#include "Track.h"
#include "Car.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    Car<int>  car;
    Vector2d<float> v1(0.f, 0.f);
    Vector2d<float> v2(1.f, 1.f);
    Vector2d<float> v3(0.f, 1.f);
    Vector2d<float> v4(1.f, 0.f);

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");
    car.initializPosition(circuit);
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        for(int i = 1; i <= 9; ++i)
            if (IsKeyPressed(i + KEY_KP_0))
            {
                car.amendVelocity(i);
                if (car.getNextPosition() != car.getPosition()
                    && car.collidesWith(circuit))
                {
                    car.setPositionToClosestPointOnCircuit(circuit);
                    car.zeroSpeed();
                }
                    car.update();

                break;
            }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            for (int i = 0; i <= screenWidth; i += 20)
            {
                DrawLine(i, 0, i, screenHeight, BLACK);
            }

            for (int j = 0; j <= screenHeight; j += 20)
            {
                DrawLine(0, j, screenWidth, j, BLACK);
            }
            circuit.render();
            DrawLine(20 * car.getPosition().x, 20 * car.getPosition().y, 20 * car.getNextPosition().x, 20 * car.getNextPosition().y, BLUE);

            DrawRectangle(20 * car.getPosition().x - 10, 20 * car.getPosition().y - 10, 20, 20, RED);

        }
        EndDrawing();

    }
}
