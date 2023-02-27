#include "stdio.h"
#include "raylib.h"
#include "lerArquivo.c"

const int screenWidth = 800;
const int screenHeight = 450;
const int linhaDiv = 15;

void ordena(float pontoCima[2], float pontoMedio[2], float pontoBaixo[2]);
void rasterizar(float P1x, float P1y, float P2x, float P2y, float P3x, float P3y);
void desenhaLinha(float P1x, float P1y, float P2x, float P2y, Color COR);
void coordenadaTela(float parametros[3][3][10000], float camera[3][6]);
void desenha(float teste[3][3][10000]);

int main(void) {
    InitWindow(screenWidth, screenHeight, "Projeto Computação Gráfica");

    float obj[3][3][10000];
    float camera[3][6];
    int arquivo = 8;

    carregaMalha(arquivo, obj);
    carregaCamera(camera);
    //coordenadaTela(obj, camera);

    int numVertices = obj[0][0][0];
    float vertices[3][10000]; 
    for (int i = 0; i < numVertices; i++) {
        vertices[0][i] = obj[1][0][i];
        vertices[1][i] = obj[1][1][i];
        vertices[2][i] = obj[1][2][i];

        printf("%f %f %f\n", vertices[0][i], vertices[1][i], vertices[2][i]);
    }
    printf("\n");

    while (!WindowShouldClose()) {

        if (IsKeyPressed('D')) {
            if (arquivo < 10) {
                arquivo++;
                carregaMalha(arquivo, obj);
            }
        }
        if (IsKeyPressed('A')) {
            if (arquivo > 3) {
                arquivo--;
                carregaMalha(arquivo, obj);
            }
        }
        if (IsKeyPressed('R')) {
            carregaMalha(arquivo, obj);
            carregaCamera(camera);
            coordenadaTela(obj, camera);
        }

        BeginDrawing();

            ClearBackground(BLACK);
            desenha(obj);

            DrawText("Aperte a ou d para navegar entre arquivos"  , 5, 2, 5, LIGHTGRAY);
            DrawText("Aperte r para aplicar a visão da camera"  , 5, 12, 5, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void desenha(float teste[3][3][10000]) {
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

    
    for (int i = 0; i < numTriangulos; i++) {
        int ver1 = triangulos[i].x - 1;
        int ver2 = triangulos[i].y - 1;
        int ver3 = triangulos[i].z - 1;

        rasterizar(vertices[ver1].x, vertices[ver1].y, vertices[ver2].x, vertices[ver2].y, vertices[ver3].x, vertices[ver3].y);
        
        //desenhaLinha(vertices[ver1].x, vertices[ver1].y, vertices[ver2].x, vertices[ver2].y, YELLOW);
        //desenhaLinha(vertices[ver1].x, vertices[ver1].y, vertices[ver3].x, vertices[ver3].y, YELLOW);
        //desenhaLinha(vertices[ver2].x, vertices[ver2].y, vertices[ver3].x, vertices[ver3].y, YELLOW);

    }

    for (int i = 0; i < numVertices; i++) {
        //DrawPixel(vertices[i].x, vertices[i].y, GREEN);
    }

}

void desenhaLinha(float P1x, float P1y, float P2x, float P2y, Color COR) {

    float auxP1x = P1x;
    float auxP1y = P1y;
    float auxP2x = P2x;
    float auxP2y = P2y;

    int deltaX = auxP2x - auxP1x;    
    int deltaY = auxP2y - auxP1y; 

    int eixoMaior;
    int eixoMenor;

    int aumentarx0 = 0;
    int aumentarY0 = 0;
    int aumentarX1 = 0;
    int aumentarY1 = 0;


    if (deltaX > 0){            
        aumentarx0 = aumentarX1 = 1;
    }
    if (deltaX < 0){     
        aumentarx0 = aumentarX1 = -1;
    }

    if (deltaY > 0){              
        aumentarY0 = 1;
    }
    if (deltaY < 0){     
        aumentarY0 = -1 ;
    }

    if (abs(deltaX) >= abs(deltaY)){     
        eixoMaior = abs(deltaX);
        eixoMenor = abs(deltaY);
    }else {
        eixoMaior = abs(deltaY);
        eixoMenor = abs(deltaX);
        if(deltaY > 0){         
            aumentarY1 = 1;
        }
        if (deltaY < 0){    
            aumentarY1 = -1;
        } 
        aumentarX1 = 0;
    }

    int numerador = eixoMaior/2;

    for (int i = 0; i <= eixoMaior; i++){
        DrawPixel(auxP1x, auxP1y, COR);

        numerador += eixoMenor;
        if (numerador > eixoMaior){
            numerador -= eixoMaior;
            auxP1x += aumentarx0;
            auxP1y += aumentarY0;
        }else {
            auxP1x += aumentarX1;
            auxP1y += aumentarY1;
        }
    } 
}

void coordenadaTela(float parametros[3][3][10000], float camera[3][6]) {
    int numVertices = parametros[0][0][0];
    float vertices[3][10000]; 
    for (int i = 0; i < numVertices; i++) {
        vertices[0][i] = parametros[1][0][i];
        vertices[1][i] = parametros[1][1][i];
        vertices[2][i] = parametros[1][2][i];
    }
    
    float C[3] = {camera[0][5], camera[1][5], camera[2][5]};
    float N[3] = {camera[0][0], camera[1][0], camera[2][0]};
    float V[3] = {camera[0][1], camera[1][1], camera[2][1]};

    float d = camera[0][2];
    float hx = camera[0][3];
    float hy = camera[0][4];

    //printf("N: %f %f %f\n", N[0], N[1], N[2]);
    //printf("V: %f %f %f\n", V[0], V[1], V[2]);
    //printf("d: %f\n", d);
    //printf("hx: %f\n", hx);
    //printf("hx: %f\n", hy);
    //printf("C: %f %f %f\n\n", C[0], C[1], C[2]);

    for (int i = 0; i < numVertices; i++) {
        float P[3] = {vertices[0][i], vertices[1][i], vertices[2][i]};
        //printf("P%i: %f %f %f\n", i, P[0], P[1], P[2]);

        //ortogonalizar o V
        float Vortogonal[3];
        float calcV = (V[0] * N[0] + V[1]*N[1] + V[2]*N[2]) / (N[0]*N[0] + N[1]*N[1] + N[2]*N[2]);
        Vortogonal[0] = V[0] - (calcV * N[0]);
        Vortogonal[1] = V[1] - (calcV * N[1]);
        Vortogonal[2] = V[2] - (calcV * N[2]);
        //printf("Vortogonal: %f %f %f\n", Vortogonal[0], Vortogonal[1], Vortogonal[2]);

        //calcular o U
        float U[3];
        U[0] = (N[1]*Vortogonal[2]) - (N[2]*Vortogonal[1]);
        U[1] = (Vortogonal[0]*N[2]) - (Vortogonal[2]*N[0]);
        U[2] = (N[0]*Vortogonal[1]) - (N[1]*Vortogonal[0]);
        //printf("U: %f %f %f\n", U[0], U[1], U[2]);

        //normalizar o U V N para U_ V_ N_
        float U_[3];
        float _U_ = sqrt( pow(U[0], 2) + pow(U[1], 2) + pow(U[2], 2) );
        U_[0] = (1/_U_) * U[0];
        U_[1] = (1/_U_) * U[1];
        U_[2] = (1/_U_) * U[2];

        float V_[3];
        float _V_ = sqrt( pow(Vortogonal[0], 2) + pow(Vortogonal[1], 2) + pow(Vortogonal[2], 2) );
        V_[0] = (1/_V_) * Vortogonal[0];
        V_[1] = (1/_V_) * Vortogonal[1];
        V_[2] = (1/_V_) * Vortogonal[2];

        float N_[3];
        float _N_ = sqrt( pow(N[0], 2) + pow(N[1], 2) + pow(N[2], 2) );
        N_[0] = (1/_N_) * N[0];
        N_[1] = (1/_N_) * N[1];
        N_[2] = (1/_N_) * N[2];
        //printf("U_: %f %f %f\n", U_[0], U_[1], U_[2]);
        //printf("V_: %f %f %f\n", V_[0], V_[1], V_[2]);
        //printf("N_: %f %f %f\n", N_[0], N_[1], N_[2]);

        //subitrair o foco do ponto P
        float P_[3] = {P[0] - C[0], P[1] - C[1], P[2] - C[2]};
        //printf("P_: %f %f %f\n", P_[0], P_[1], P_[2]);

        //transformar no sistema de vista
        float Pvista[3];
        Pvista[0] = U_[0]*P_[0] + U_[1]*P_[1] + U_[2]*P_[2];
        Pvista[1] = V_[0]*P_[0] + V_[1]*P_[1] + V_[2]*P_[2];
        Pvista[2] = N_[0]*P_[0] + N_[1]*P_[1] + N_[2]*P_[2];
        //printf("sistema de vista: %f %f %f\n", Pvista[0], Pvista[1], Pvista[2]);

        //projeção em perspectiva
        float projPersp[2];
        projPersp[0] = d * (Pvista[0] / Pvista[2]);
        projPersp[1] = d * (Pvista[1] / Pvista[2]);
        //printf("projeção em perspectiva: %f %f\n", projPersp[0], projPersp[1]);

        //coordenadas normalizadas
        float cordNormalizada[2];
        cordNormalizada[0] = projPersp[0]/hx;
        cordNormalizada[1] = projPersp[1]/hy;
        //printf("projeção normalizada: %f %f\n", cordNormalizada[0], cordNormalizada[1]);

        //coordenas de tela
        float pTela[2];

        pTela[0] = round(((cordNormalizada[0] + 1)/2) * screenWidth + 0.5);
        pTela[1] = round(screenHeight - (((cordNormalizada[1] + 1)/2) * screenHeight + 0.5));
        //printf("Coordenas de tela: %f %f\n\n", pTela[0], pTela[1]);

        parametros[1][0][i] = pTela[0];
        parametros[1][1][i] = pTela[1];
    }

}

void rasterizar(float P1x, float P1y, float P2x, float P2y, float P3x, float P3y) {
    float pontoCima[2] = {P1x, P1y};
    float pontoMedio[2] = {P2x, P2y};
    float pontoBaixo[2] = {P3x, P3y};

//Ordena os pontos do mais alto para o mais baixo
    ordena(pontoCima, pontoMedio, pontoBaixo);

    printf("PontoCima X: %f PontoCima Y: %f\n", pontoCima[0], pontoCima[1]);
    printf("pontoMedio X: %f pontoMedio Y: %f\n", pontoMedio[0], pontoMedio[1]);
    printf("pontoBaixo X: %f pontoBaixo Y: %f\n\n", pontoBaixo[0], pontoBaixo[1]);

    float Yscan = pontoCima[1];
    float Xmin = pontoCima[0];
    float Xmax = pontoCima[0];

    float aEsq;
    float aDir;

//garante uma inclinação valida 
    float temp = (pontoMedio[0] - pontoCima[0]);
    if (temp == 0) {
        aEsq = 0;
    }else {
        aEsq = (int) (pontoMedio[1] - pontoCima[1])/(pontoMedio[0] - pontoCima[0]); 
        if (!aEsq == 0) {
            aEsq = (1/aEsq);
        }
    }
    temp = (pontoBaixo[0] - pontoCima[0]);
    if (temp == 0) {
        aDir = 0;
    }else {
        aDir = (int) (pontoBaixo[1] - pontoCima[1])/(pontoBaixo[0] - pontoCima[0]);
        if (!aDir == 0) {
            aDir = (1/aDir);
        }
    }

    if(pontoMedio[0] > pontoBaixo[0]) {
        float troca = aEsq;
        aEsq = aDir;
        aDir = troca;
    }
    
    printf("aEsq: %f aDir: %f\n\n", aEsq, aDir);

    printf("Xmin: %f Xmax: %f\n\n", Xmin, Xmax);


    if(aEsq == 0 && aDir == 0) {
        Xmax = pontoBaixo[0];
    }

    while (Yscan <= pontoMedio[1]) {
        float Xmin_ = (Xmin + aEsq);
        float Xmax_ = (Xmax + aDir);

        printf("Xmin: %f Xmax: %f\n", round(Xmin), round(Xmax));
        printf("Xmin_: %f Xmax_: %f\n\n", round(Xmin_), round(Xmax_));
        desenhaLinha(round(Xmin), Yscan, round(Xmax), Yscan, BLUE);

        Yscan++;
        Xmin = Xmin_;
        Xmax = Xmax_;
    }
//----------------------------------------------------------------

// define o ponto medio D
    float pontoD[2];
    if(pontoMedio[0] > pontoBaixo[0]) {
        pontoD[0] = (Xmin - aEsq); 
        pontoD[1] = pontoMedio[1];
    }else {
        pontoD[0] = (Xmax - aDir); 
        pontoD[1] = pontoMedio[1];
    }
    printf("pontoD X: %f pontoD Y: %f\n\n", pontoD[0], pontoD[1]);

    Yscan = pontoBaixo[1];
    Xmin = pontoBaixo[0];
    Xmax = pontoBaixo[0];

    //garante uma inclinação valida 
    temp = (pontoMedio[0] - pontoBaixo[0]);
    if (temp == 0) {
        aEsq = 0;
    }else {
        aEsq = (int) (pontoMedio[1] - pontoBaixo[1])/(pontoMedio[0] - pontoBaixo[0]); 
        if (!aEsq == 0) {
            aEsq = (1/aEsq);
        }
    }
    temp = (pontoD[0] - pontoBaixo[0]);
    if (temp == 0) {
        aDir = 0;
    }else {
        aDir = (int) (pontoD[1] - pontoBaixo[1])/(pontoD[0] - pontoBaixo[0]);
        if (!aDir == 0) {
            aDir = (1/aDir);
        }
    }
    printf("aEsq: %f aDir: %f\n", aEsq, aDir);
    if(pontoMedio[0] > pontoD[0]) {
        float troca = aEsq;
        aEsq = aDir;
        aDir = troca;
    }

    printf("aEsq: %f aDir: %f\n\n", aEsq, aDir);

    printf("Xmin: %f Xmax: %f\n\n", Xmin, Xmax);

    if(aEsq == 0 && aDir == 0) {
        Xmax = pontoMedio[0];
    }

    while (Yscan >= pontoMedio[1]) {
        float Xmin_ = (Xmin - aEsq);
        float Xmax_ = (Xmax - aDir);

        printf("2Xmin: %f Xmax: %f\n", round(Xmin), round(Xmax));
        printf("2Xmin_: %f Xmax_: %f\n\n", round(Xmin_), round(Xmax_));
        desenhaLinha(round(Xmin), Yscan, round(Xmax), Yscan, BLUE);

        Yscan--;
        Xmin = Xmin_;
        Xmax = Xmax_;
    }
}

void ordena(float pontoCima[2], float pontoMedio[2], float pontoBaixo[2]) {
    float ponto1X = pontoCima[0];
    float ponto1Y = pontoCima[1];
    float ponto2X = pontoMedio[0];
    float ponto2Y = pontoMedio[1];
    float ponto3X = pontoBaixo[0];
    float ponto3Y = pontoBaixo[1];

    if(ponto1Y > ponto3Y) {
        float tmp = ponto3Y;
        ponto3Y = ponto1Y;
        ponto1Y = tmp;

        tmp = ponto3X;
        ponto3X = ponto1X;
        ponto1X = tmp;
    }
    if(ponto1Y > ponto2Y) {
        float tmp = ponto2Y;
        ponto2Y = ponto1Y;
        ponto1Y = tmp;

        tmp = ponto2X;
        ponto2X = ponto1X;
        ponto1X = tmp;
    }
    if(ponto2Y > ponto3Y) {
        float tmp = ponto3Y;
        ponto3Y = ponto2Y;
        ponto2Y = tmp;

        tmp = ponto3X;
        ponto3X = ponto2X;
        ponto2X = tmp;
    }

    if(ponto1Y == ponto3Y) {
        if(ponto1X > ponto3X) {
            float tmp = ponto3Y;
            ponto3Y = ponto1Y;
            ponto1Y = tmp;

            tmp = ponto3X;
            ponto3X = ponto1X;
            ponto1X = tmp;
        }
    }
    if(ponto1Y == ponto2Y) {
        if(ponto1X > ponto2X) {
            float tmp = ponto2Y;
            ponto2Y = ponto1Y;
            ponto1Y = tmp;

            tmp = ponto2X;
            ponto2X = ponto1X;
            ponto1X = tmp;
        }
    }
    if(ponto2Y == ponto3Y) {
        if(ponto2X > ponto3X) {
            float tmp = ponto3Y;
            ponto3Y = ponto2Y;
            ponto2Y = tmp;

            tmp = ponto3X;
            ponto3X = ponto2X;
            ponto2X = tmp;
        }
    }

    pontoCima[0] = ponto1X;
    pontoCima[1] = ponto1Y;
    pontoMedio[0] = ponto2X;
    pontoMedio[1] = ponto2Y;
    pontoBaixo[0] = ponto3X;
    pontoBaixo[1] = ponto3Y;
}

