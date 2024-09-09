#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdlib.h>
#include <stdio.h>


#define HEIGHT 720
#define WIDTH 1280

#define CAMERA_SPEED 0.01
#define ZOOM_SPEED 1

#define SCALE 1
#define ORIGIN_OFFSET ((float)SCALE / 2)

Vector3 target = {0};
Vector3 origin = {-ORIGIN_OFFSET, -ORIGIN_OFFSET, -ORIGIN_OFFSET};
Vector3 camera_start_pos = {10, 10, 10};
Vector3 camera_start_up = {0, 1, 0};

typedef struct {
    Vector3 p[8];
    float val[8];
} Cube;

void draw_grid(int width, int height, int depth) {

    for (size_t z = 0; z < depth; z++) {
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {

                int offset_z = z - (depth - 1) / 2.0;
                int offset_y = y - (height - 1) / 2.0;
                int offset_x = x - (width - 1) / 2.0;

                DrawSphere((Vector3){offset_x, offset_y, offset_z}, .05, GRAY);
            }
        }
    }
}

void generate_cubes(Cube *cubes) {
    cubes[0].p[0] = (Vector3){1, 1, 1};
    cubes[0].p[1] = (Vector3){1, 1, -1};
    cubes[0].p[2] = (Vector3){1, -1, 1};
    cubes[0].p[3] = (Vector3){1, -1, -1};
    cubes[0].p[4] = (Vector3){-1, 1, 1};
    cubes[0].p[5] = (Vector3){-1, 1, -1};
    cubes[0].p[6] = (Vector3){-1, -1, 1};
    cubes[0].p[7] = (Vector3){-1, -1, -1};


    cubes[0].val[0] = 0;
    cubes[0].val[1] = 0;
    cubes[0].val[2] = 0;
    cubes[0].val[3] = 255;
    cubes[0].val[4] = 0;
    cubes[0].val[5] = 0;
    cubes[0].val[6] = 0;
    cubes[0].val[7] = 255;
}

void print_vec(Vector3 vec) {
    printf("Cube 0, vertex 0: (%f, %f, %f)\n", vec.x, vec.y, vec.z);
}

void draw_cube_vertex(Cube cube) {

    DrawSphere((Vector3){cube.p[0].x, cube.p[0].y, cube.p[0].z}, .05, (Color){cube.val[0], cube.val[0], cube.val[0], 255});
    DrawSphere((Vector3){cube.p[1].x, cube.p[1].y, cube.p[1].z}, .05, (Color){cube.val[1], cube.val[1], cube.val[1], 255});
    DrawSphere((Vector3){cube.p[2].x, cube.p[2].y, cube.p[2].z}, .05, (Color){cube.val[2], cube.val[2], cube.val[2], 255});
    DrawSphere((Vector3){cube.p[3].x, cube.p[3].y, cube.p[3].z}, .05, (Color){cube.val[3], cube.val[3], cube.val[3], 255});
    DrawSphere((Vector3){cube.p[4].x, cube.p[4].y, cube.p[4].z}, .05, (Color){cube.val[4], cube.val[4], cube.val[4], 255});
    DrawSphere((Vector3){cube.p[5].x, cube.p[5].y, cube.p[5].z}, .05, (Color){cube.val[5], cube.val[5], cube.val[5], 255});
    DrawSphere((Vector3){cube.p[6].x, cube.p[6].y, cube.p[6].z}, .05, (Color){cube.val[6], cube.val[6], cube.val[6], 255});
    DrawSphere((Vector3){cube.p[7].x, cube.p[7].y, cube.p[7].z}, .05, (Color){cube.val[7], cube.val[7], cube.val[7], 255});
}

Mesh draw_triangle(Cube cube) {
    Mesh mesh = {0};

    mesh.vertexCount = 3;
    mesh.triangleCount = 1;

    mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));

    mesh.vertices[0] = 0.0f; mesh.vertices[1] = 1.0f; mesh.vertices[2] = 0.0f;  // Vertex 1 (top)
    mesh.vertices[3] = 1.0f; mesh.vertices[4] = -1.0f; mesh.vertices[5] = 0.0f; // Vertex 2 (right)
    mesh.vertices[6] = -1.0f; mesh.vertices[7] = -1.0f; mesh.vertices[8] = 0.0f; // Vertex 3 (left)

    UploadMesh(&mesh, false);

    return mesh;
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "MarchingCubes");
    SetTargetFPS(60);

    Cube cubes[1];
    generate_cubes(cubes);

    Mesh mesh = draw_triangle(cubes[0]);
    
    Material matt = LoadMaterialDefault(); 
    matt.maps[MATERIAL_MAP_DIFFUSE].color = RED;

    print_vec(cubes[0].p[0]);

    int grid_width = 5;
    int grid_height = 5;
    int grid_depth = 5;

    Camera3D camera = {
        .position = camera_start_pos,
        .target = target,
        .up = camera_start_up,
        .fovy = 30,
        .projection = CAMERA_PERSPECTIVE};


    while (!WindowShouldClose()) {

        float dist = GetMouseWheelMove();
        CameraMoveToTarget(&camera, -dist * ZOOM_SPEED);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mouseDelta = GetMouseDelta();
            Vector2 mousePos = GetMousePosition();
            if (mousePos.x > 70) {
                CameraYaw(&camera, -mouseDelta.x * CAMERA_SPEED, true);
                CameraPitch(&camera, -mouseDelta.y * CAMERA_SPEED, true, true, false);
            }
        }

        if (IsKeyPressed(KEY_R)) {
            camera.position = camera_start_pos;
            camera.up = camera_start_up;
        }

        // clang-format off
        BeginDrawing();
            ClearBackground(GetColor(0x181818AA));
                BeginMode3D(camera);
                draw_cube_vertex(cubes[0]);
                // draw_triangle(cubes[0]);
                // draw_grid(grid_width, grid_height, grid_depth);

                DrawMesh(mesh, matt, MatrixIdentity());
                EndMode3D();
        EndDrawing();
        // clang-format on
    }

    CloseWindow();

    return 0;
}