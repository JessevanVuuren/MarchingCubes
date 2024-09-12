#include "maps.h"
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

    for (size_t z = 0; z < depth + 1; z++) {
        for (size_t y = 0; y < height + 1; y++) {
            for (size_t x = 0; x < width + 1; x++) {

                int offset_z = z - (depth - 1) / 2.0;
                int offset_y = y - (height - 1) / 2.0;
                int offset_x = x - (width - 1) / 2.0;

                DrawSphere((Vector3){offset_x - .5, offset_y - 0.5, offset_z - 0.5}, .05, GRAY);
            }
        }
    }
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

void draw_cubes_vertex(Cube *cubes, int width, int height, int depth) {
    for (size_t i = 0; i < width * height * depth; i++) {
        draw_cube_vertex(cubes[i]);
    }
}

void generate_cubes(Cube *cubes, int width, int height, int depth) {
    int index = 0;
    for (size_t z = 0; z < depth; z++) {
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                int offset_z = z - (depth - 1) / 2.0;
                int offset_y = y - (height - 1) / 2.0;
                int offset_x = x - (width - 1) / 2.0;

                Cube *c = &cubes[index];

                c->p[0] = (Vector3){offset_x + -.5, offset_y + -.5, -.5 + offset_z};
                c->p[1] = (Vector3){offset_x + .5, offset_y + -.5, -.5 + offset_z};
                c->p[2] = (Vector3){offset_x + .5, offset_y + -.5, .5 + offset_z};
                c->p[3] = (Vector3){offset_x + -.5, offset_y + -.5, .5 + offset_z};
                c->p[4] = (Vector3){offset_x + -.5, offset_y + .5, -.5 + offset_z};
                c->p[5] = (Vector3){offset_x + .5, offset_y + .5, -.5 + offset_z};
                c->p[6] = (Vector3){offset_x + .5, offset_y + .5, .5 + offset_z};
                c->p[7] = (Vector3){offset_x + -.5, offset_y + .5, .5 + offset_z};

                memset(c->val, 0, sizeof(c->val));

                index++;
            }
        }
    }
}

void print_vec(Vector3 vec) {
    printf("Cube 0, vertex 0: (%f, %f, %f)\n", vec.x, vec.y, vec.z);
}

Vector3 center_of_two_vec3(Vector3 point1, Vector3 point2) {
    float center_x = (point1.x + point2.x) / 2;
    float center_y = (point1.y + point2.y) / 2;
    float center_z = (point1.z + point2.z) / 2;

    return (Vector3){center_x, center_y, center_z};
}

Mesh draw_mesh(Cube cube) {
    Vector3 vert_list[12];

    int cube_index = 0;
    if (cube.val[0] < 100) cube_index |= 1;
    if (cube.val[1] < 100) cube_index |= 2;
    if (cube.val[2] < 100) cube_index |= 4;
    if (cube.val[3] < 100) cube_index |= 8;
    if (cube.val[4] < 100) cube_index |= 16;
    if (cube.val[5] < 100) cube_index |= 32;
    if (cube.val[6] < 100) cube_index |= 64;
    if (cube.val[7] < 100) cube_index |= 128;

    if (edgeTable[cube_index] & 1) vert_list[0] = center_of_two_vec3(cube.p[0], cube.p[1]);
    if (edgeTable[cube_index] & 2) vert_list[1] = center_of_two_vec3(cube.p[1], cube.p[2]);
    if (edgeTable[cube_index] & 4) vert_list[2] = center_of_two_vec3(cube.p[2], cube.p[3]);
    if (edgeTable[cube_index] & 8) vert_list[3] = center_of_two_vec3(cube.p[3], cube.p[0]);
    if (edgeTable[cube_index] & 16) vert_list[4] = center_of_two_vec3(cube.p[4], cube.p[5]);
    if (edgeTable[cube_index] & 32) vert_list[5] = center_of_two_vec3(cube.p[5], cube.p[6]);
    if (edgeTable[cube_index] & 64) vert_list[6] = center_of_two_vec3(cube.p[6], cube.p[7]);
    if (edgeTable[cube_index] & 128) vert_list[7] = center_of_two_vec3(cube.p[7], cube.p[4]);
    if (edgeTable[cube_index] & 256) vert_list[8] = center_of_two_vec3(cube.p[0], cube.p[4]);
    if (edgeTable[cube_index] & 512) vert_list[9] = center_of_two_vec3(cube.p[1], cube.p[5]);
    if (edgeTable[cube_index] & 1024) vert_list[10] = center_of_two_vec3(cube.p[2], cube.p[6]);
    if (edgeTable[cube_index] & 2048) vert_list[11] = center_of_two_vec3(cube.p[3], cube.p[7]);

    int vertex_count = 0;
    printf("on: %d, on: %d\n", cube_index, vertex_count);
    for (; triTable[cube_index][vertex_count] != -1; vertex_count++);

    Mesh mesh = {0};
    mesh.vertexCount = vertex_count * 2;
    mesh.triangleCount = mesh.vertexCount / 3.0;
    mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));

    printf("original triangles: %d\n", vertex_count);
    printf("vertexCount: %d\n", mesh.vertexCount);
    printf("triangleCount: %d\n", mesh.triangleCount);

    int current_triangle = 0;
    for (int i = 0; i < vertex_count; i += 3) {
        // Print each vertex for debugging (optional)
        print_vec(vert_list[triTable[cube_index][i]]);
        print_vec(vert_list[triTable[cube_index][i + 1]]);
        print_vec(vert_list[triTable[cube_index][i + 2]]);


        // Reverse the vertex order by assigning in reverse
        mesh.vertices[current_triangle] = vert_list[triTable[cube_index][i + 2]].x;
        mesh.vertices[current_triangle + 1] = vert_list[triTable[cube_index][i + 2]].y;
        mesh.vertices[current_triangle + 2] = vert_list[triTable[cube_index][i + 2]].z;

        mesh.vertices[current_triangle + 3] = vert_list[triTable[cube_index][i + 1]].x;
        mesh.vertices[current_triangle + 4] = vert_list[triTable[cube_index][i + 1]].y;
        mesh.vertices[current_triangle + 5] = vert_list[triTable[cube_index][i + 1]].z;

        mesh.vertices[current_triangle + 6] = vert_list[triTable[cube_index][i]].x;
        mesh.vertices[current_triangle + 7] = vert_list[triTable[cube_index][i]].y;
        mesh.vertices[current_triangle + 8] = vert_list[triTable[cube_index][i]].z;

        // Move to the next triangle
        current_triangle += 9; // 3 vertices * 3 coordinates (x, y, z)
    }

    for (int i = 0; i < vertex_count; i++) {
        print_vec(vert_list[triTable[cube_index][i]]);
        mesh.vertices[current_triangle + 0] = vert_list[triTable[cube_index][i]].x;
        mesh.vertices[current_triangle + 1] = vert_list[triTable[cube_index][i]].y;
        mesh.vertices[current_triangle + 2] = vert_list[triTable[cube_index][i]].z;
        current_triangle += 3;
    }

    UploadMesh(&mesh, false);
    return mesh;
}


int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "MarchingCubes");
    SetTargetFPS(120);

    int grid_width = 5;
    int grid_height = 5;
    int grid_depth = 5;

    int total_size = grid_width * grid_height * grid_depth;

    Cube *cubes = (Cube *)malloc(total_size * sizeof(Cube));
    generate_cubes(cubes, grid_width, grid_height, grid_depth);

    cubes[0].val[0] = 255; // test mesh

    Mesh mesh = draw_mesh(cubes[0]);
    Model model = LoadModelFromMesh(mesh);

    Material matt = LoadMaterialDefault();
    matt.maps[MATERIAL_MAP_DIFFUSE].color = RED;

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
    
                draw_cubes_vertex(cubes, grid_width, grid_height, grid_depth);
                // draw_grid(grid_width, grid_height, grid_depth);
                
                DrawMesh(mesh, matt, MatrixIdentity());
                DrawModelWires(model, Vector3Zero(), 1, WHITE);

                EndMode3D();
            DrawFPS(10, 10);

        EndDrawing();
        // clang-format on
    }

    CloseWindow();

    return 0;
}