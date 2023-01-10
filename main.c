#include "stdio.h"
#include "raylib.h"

int comparaFloats (float f1, float f2);
void desenhaLinha(Vector3 vec1, Vector3 vec2);
void desenhaTriangulos(Vector3 vertices[], int numVertices, Vector3 triangulos[], int numTriangulos);

const float dist = 0.001;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Projeto Computação Gráfica");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;  
    
    SetCameraMode(camera, CAMERA_FREE);


    int numVertices = 3;
    Vector3 vertices[numVertices]; 

    vertices[0] = (Vector3){ 5.0f, 0.0f, 0.0f };
    vertices[1] = (Vector3){ 0.0f, 5.0f, 0.0f };
    vertices[2] = (Vector3){ 0.0f, 0.0f, 5.0f };


    int numTriangulos = 1;
    Vector3 triangulos[numTriangulos]; 

    triangulos[0] = (Vector3){ 1.0f, 2.0f, 3.0f };


    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        UpdateCamera(&camera);

        if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(camera);

            desenhaTriangulos(vertices, numVertices, triangulos, numTriangulos);   

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void desenhaTriangulos(Vector3 vertices[], int numVertices, Vector3 triangulos[], int numTriangulos) {
    for (int i = 0; i < numVertices; i++) {
        DrawCube(vertices[i], 0.02, 0.02, 0.02, GREEN);
    }

    for (int i = 0; i < numTriangulos; i++) {
        int ver1 = triangulos[i].x - 1;
        int ver2 = triangulos[i].y - 1;
        int ver3 = triangulos[i].z - 1;

        desenhaLinha(vertices[ver1] , vertices[ver2]);        
        desenhaLinha(vertices[ver1] , vertices[ver3]);        
        desenhaLinha(vertices[ver2] , vertices[ver3]);        
    }
}

void desenhaLinha(Vector3 vec1, Vector3 vec2) {
    float x1 = vec1.x;
    float y1 = vec1.y;
    float z1 = vec1.z;

    float x2 = vec2.x;
    float y2 = vec2.y;
    float z2 = vec2.z;

    while (comparaFloats(x1, x2) || comparaFloats(y1, y2) || comparaFloats(z1, z2)) {

        if (x1 > x2) {
            x1 = x1 - dist;
        }else if (x1 < x2) {
            x1 = x1 + dist;
        }

        if (y1 > y2) {
            y1 = y1 - dist;
        }else if (y1 < y2) {
            y1 = y1 + dist;
        }
        
        if (z1 > z2) {
            z1 = z1 - dist;
        }else if (z1 < z2) {
            z1 = z1 + dist;
        }

        DrawCube((Vector3){ x1, y1, z1 }, 0.02, 0.02, 0.02, GREEN);
    }
}

int comparaFloats (float f1, float f2) {
    float precision = dist;
    
    if (((f1 - precision) < f2) && ((f1 + precision) > f2)) {
        return 0;
    }else {
        return 1;
   }

 }
