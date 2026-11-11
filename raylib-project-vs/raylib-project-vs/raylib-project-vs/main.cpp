#include "raylib.h"
#include <vector>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

void Input()
{

}

void Update()
{

}

void Draw()
{
    BeginDrawing();
    ClearBackground(WHITE);

    DrawText("This is a Raylib Starter Project for C++ Coderdojo", SCREEN_WIDTH / 2 - 325, SCREEN_HEIGHT / 2 - 13, 26, BLUE);

    EndDrawing();
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Starter Template");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Input();
        Update();
        Draw();
    }
    CloseWindow();
    return 0;
}