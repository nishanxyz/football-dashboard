#include <raylib.h>
#include "apiconnect.h"
#include <iostream>
using namespace std;
int main()
{
    InitWindow(1280, 720, "Football Dashboard");

    Font titleFont = LoadFontEx("E:/New folder/holborn/Holborn.otf", 20, NULL, 0);
    Vector2 position = { 600, 15 };
    Color c = { 29, 36, 33, 255 };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextEx(titleFont, "Football Dashboard", position, 20, 1, RAYWHITE);
        DrawText("LIVE MATCHES :", 11, 52, 20, RAYWHITE);
        Vector2 textSize = MeasureTextEx(titleFont, "Football Dashboard", 20, 1);
        DrawLine(600, 15 + 20 + 4, 600 + textSize.x, 15 + 20 + 4, RAYWHITE);
        EndDrawing();
    }

    UnloadFont(titleFont);
    CloseWindow();

    return 0;
}