#include "stdio.h"
#include "raylib.h"
#include "lerArquivo.c"

const float dist = 0.5;
const float quadradoTam = 1;

int comparaFloats (float f1, float f2);
void desenhaLinha(Vector3 vec1, Vector3 vec2);
void desenhaTriangulos(float teste[3][3][10000]);

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    int arquivo = 7;
    InitWindow(screenWidth, screenHeight, "Projeto Computação Gráfica");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 500.0f, 500.0f, 500.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;  
    
    SetCameraMode(camera, CAMERA_FREE);

    float teste[3][3][10000];

    while (!WindowShouldClose()) {
        UpdateCamera(&camera);
        char filename[100];
        sprintf(filename, "%i", arquivo);
        if (IsKeyPressed('D')) arquivo++;
        if (IsKeyPressed('A')) arquivo--;
        carregaMalha(arquivo, teste);


        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(camera);

            desenhaTriangulos(teste);

            EndMode3D();

            DrawFPS(10, 10);
            DrawText(filename, 240, 260, 20, LIGHTGRAY);

        EndDrawing();

        if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

    }
    CloseWindow();

    return 0;
}

void desenhaTriangulos(float teste[3][3][10000]) {
    int numVertices = teste[0][0][0];
    Vector3 vertices[10000]; 
    for (int i = 0; i < numVertices; i++) {
        vertices[i] = (Vector3){ teste[1][0][i], teste[1][1][i], teste[1][2][i] };
    }

    int numTriangulos = teste[0][1][0];
    Vector3 triangulos[10000]; 
    for (int i = 0; i < numTriangulos; i++) {
        triangulos[i] = (Vector3){ teste[2][0][i], teste[2][1][i], teste[2][2][i] };    
    }


    for (int i = 0; i < numVertices; i++) {
        DrawCube(vertices[i], quadradoTam, quadradoTam, quadradoTam, GREEN);
    }

    for (int i = 0; i < numTriangulos; i++) {
        int ver1 = triangulos[i].x - 1;
        int ver2 = triangulos[i].y - 1;
        int ver3 = triangulos[i].z - 1;

        //desenhaLinha(vertices[ver1] , vertices[ver2]);        
        //desenhaLinha(vertices[ver1] , vertices[ver3]);        
        //desenhaLinha(vertices[ver2] , vertices[ver3]);     
        DrawLine3D(vertices[ver1], vertices[ver2], RED);   
        DrawLine3D(vertices[ver1], vertices[ver3], RED);   
        DrawLine3D(vertices[ver2], vertices[ver3], RED);   
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

        dDrawCube((Vector3){ x1, y1, z1 }, quadradoTam, quadradoTam, quadradoTam, GREEN);
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
