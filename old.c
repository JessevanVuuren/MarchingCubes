
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