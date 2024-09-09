#include <raylib.h>

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "MarchingCubes");

    SetTargetFPS(60);              
   
    while (!WindowShouldClose())    
    {
       
        BeginDrawing();

            ClearBackground(GetColor(0x181818AA));

            DrawText("Marching cubes", 190, 200, 20, GetColor(0xffdd33FF));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}