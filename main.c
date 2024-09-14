#include "maps.h"
#include "perlinNoise.h"

#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HEIGHT 720
#define WIDTH 1280

#define CAMERA_SPEED 0.01
#define ZOOM_SPEED 1

#define SCALE 1
#define ORIGIN_OFFSET ((float)SCALE / 2)

double size = 1;

int *permutation_table;

Vector3 target = {0};
Vector3 origin = {-ORIGIN_OFFSET, -ORIGIN_OFFSET, -ORIGIN_OFFSET};
Vector3 camera_start_pos = {10, 10, 10};
Vector3 camera_start_up = {0, 1, 0};

typedef struct {
    Vector3 p[8];
    double val[8];
} Cube;

void draw_grid(int width, int height, int depth) {
    double time = GetTime() * 2;

    for (size_t z = 0; z < depth + 1; z++) {
        for (size_t y = 0; y < height + 1; y++) {
            for (size_t x = 0; x < width + 1; x++) {

                int offset_z = z - (depth - 1) / 2.0;
                int offset_y = y - (height - 1) / 2.0;
                int offset_x = x - (width - 1) / 2.0;

                double noise_x = x / (double)width * size;
                double noise_y = y / (double)height * size;
                double noise_z = z / (double)depth * size;

                double val = Noise3D(noise_x, noise_y, noise_z, permutation_table);
                // int color = (int)floor(((val + 1) * 127.5));
                int color = val > 0 ? 0 : 255;

                DrawSphere((Vector3){offset_x - .5, offset_y - 0.5, offset_z - 0.5}, .03, (Color){color, color, color, 255});
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

                float offset_z = z - (depth - 1) / 2.0;
                float offset_y = y - (height - 1) / 2.0;
                float offset_x = x - (width - 1) / 2.0;

                Cube *c = &cubes[index];

                Vector3 pos0 = (Vector3){offset_x + -.5, offset_y + -.5, -.5 + offset_z};
                Vector3 pos1 = (Vector3){offset_x + .5, offset_y + -.5, -.5 + offset_z};
                Vector3 pos2 = (Vector3){offset_x + .5, offset_y + -.5, .5 + offset_z};
                Vector3 pos3 = (Vector3){offset_x + -.5, offset_y + -.5, .5 + offset_z};
                Vector3 pos4 = (Vector3){offset_x + -.5, offset_y + .5, -.5 + offset_z};
                Vector3 pos5 = (Vector3){offset_x + .5, offset_y + .5, -.5 + offset_z};
                Vector3 pos6 = (Vector3){offset_x + .5, offset_y + .5, .5 + offset_z};
                Vector3 pos7 = (Vector3){offset_x + -.5, offset_y + .5, .5 + offset_z};

                c->p[0] = pos0;
                c->p[1] = pos1;
                c->p[2] = pos2;
                c->p[3] = pos3;
                c->p[4] = pos4;
                c->p[5] = pos5;
                c->p[6] = pos6;
                c->p[7] = pos7;

                c->val[0] = Noise3D(pos0.x / (double)width * size, pos0.y / (double)height * size, pos0.z / (double)depth * size, permutation_table) > 0 ? 0 : 255;
                c->val[1] = Noise3D(pos1.x / (double)width * size, pos1.y / (double)height * size, pos1.z / (double)depth * size, permutation_table) > 0 ? 0 : 255;
                c->val[2] = Noise3D(pos2.x / (double)width * size, pos2.y / (double)height * size, pos2.z / (double)depth * size, permutation_table) > 0 ? 0 : 255;
                c->val[3] = Noise3D(pos3.x / (double)width * size, pos3.y / (double)height * size, pos3.z / (double)depth * size, permutation_table) > 0 ? 0 : 255;
                c->val[4] = Noise3D(pos4.x / (double)width * size, pos4.y / (double)height * size, pos4.z / (double)depth * size, permutation_table) > 0 ? 0 : 255;
                c->val[5] = Noise3D(pos5.x / (double)width * size, pos5.y / (double)height * size, pos5.z / (double)depth * size, permutation_table) > 0 ? 0 : 255;
                c->val[6] = Noise3D(pos6.x / (double)width * size, pos6.y / (double)height * size, pos6.z / (double)depth * size, permutation_table) > 0 ? 0 : 255;
                c->val[7] = Noise3D(pos7.x / (double)width * size, pos7.y / (double)height * size, pos7.z / (double)depth * size, permutation_table) > 0 ? 0 : 255;

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

Mesh draw_mesh(Cube *cubes, int width, int height, int depth, Vector3 *triangles) {
    int current_triangle = 0;
    
    for (size_t i = 0; i < width * height * depth; i++) {
        Cube *c = &cubes[i];

        Vector3 vert_list[12];

        int cube_index = 0;
        if (c->val[0] < 100) cube_index |= 1;
        if (c->val[1] < 100) cube_index |= 2;
        if (c->val[2] < 100) cube_index |= 4;
        if (c->val[3] < 100) cube_index |= 8;
        if (c->val[4] < 100) cube_index |= 16;
        if (c->val[5] < 100) cube_index |= 32;
        if (c->val[6] < 100) cube_index |= 64;
        if (c->val[7] < 100) cube_index |= 128;

        if (edgeTable[cube_index] & 1) vert_list[0] = center_of_two_vec3(c->p[0], c->p[1]);
        if (edgeTable[cube_index] & 2) vert_list[1] = center_of_two_vec3(c->p[1], c->p[2]);
        if (edgeTable[cube_index] & 4) vert_list[2] = center_of_two_vec3(c->p[2], c->p[3]);
        if (edgeTable[cube_index] & 8) vert_list[3] = center_of_two_vec3(c->p[3], c->p[0]);
        if (edgeTable[cube_index] & 16) vert_list[4] = center_of_two_vec3(c->p[4], c->p[5]);
        if (edgeTable[cube_index] & 32) vert_list[5] = center_of_two_vec3(c->p[5], c->p[6]);
        if (edgeTable[cube_index] & 64) vert_list[6] = center_of_two_vec3(c->p[6], c->p[7]);
        if (edgeTable[cube_index] & 128) vert_list[7] = center_of_two_vec3(c->p[7], c->p[4]);
        if (edgeTable[cube_index] & 256) vert_list[8] = center_of_two_vec3(c->p[0], c->p[4]);
        if (edgeTable[cube_index] & 512) vert_list[9] = center_of_two_vec3(c->p[1], c->p[5]);
        if (edgeTable[cube_index] & 1024) vert_list[10] = center_of_two_vec3(c->p[2], c->p[6]);
        if (edgeTable[cube_index] & 2048) vert_list[11] = center_of_two_vec3(c->p[3], c->p[7]);

        int vertex_count = 0;
        for (; triTable[cube_index][vertex_count] != -1; vertex_count++);

        for (int i = 0; i < vertex_count; i += 3) {
            triangles[current_triangle + 0] = vert_list[triTable[cube_index][i + 2]];
            triangles[current_triangle + 1] = vert_list[triTable[cube_index][i + 1]];
            triangles[current_triangle + 2] = vert_list[triTable[cube_index][i + 0]];
            current_triangle += 3;
        }

        for (int i = 0; i < vertex_count; i++) {
            triangles[current_triangle] = vert_list[triTable[cube_index][i]];
            current_triangle++;
        }
    }
}

Mesh build_mesh(Vector3 *triangles, int size) {
    Mesh mesh = {0};
    mesh.vertexCount = size;
    mesh.triangleCount = mesh.vertexCount / 3.0;
    mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));

    for (size_t i = 0; i < size; i++)
    {
        print_vec(triangles[i]);
    }
    

    int triangleCount = 0;
    for (size_t i = 0; i < size; i++) {
        mesh.vertices[triangleCount + 0] = triangles[i].x;
        mesh.vertices[triangleCount + 1] = triangles[i].y;
        mesh.vertices[triangleCount + 2] = triangles[i].z;

        triangleCount+=3;
    }

    UploadMesh(&mesh, false);
    return mesh;
}


int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "MarchingCubes");
    SetTargetFPS(120);

    srand(2);

    permutation_table = make_permutation();

    int grid_width = 2;
    int grid_height = 1;
    int grid_depth = 1;

    int total_size = grid_width * grid_height * grid_depth;

    Cube *cubes = (Cube *)malloc(total_size * sizeof(Cube));
    generate_cubes(cubes, grid_width, grid_height, grid_depth);

    int sizeE = 2 * 15;

    Vector3 *triangles = (Vector3 *)malloc(sizeE * sizeof(Vector3));
    draw_mesh(cubes, grid_width, grid_height, grid_depth, triangles);

    Mesh mesh = build_mesh(triangles, sizeE);
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