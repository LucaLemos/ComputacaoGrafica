#include "stdio.h"
#include "raylib.h"
#include "lerArquivo.c"

const int screenWidth = 800;
const int screenHeight = 450;
const int linhaDiv = 15;

void ordena(float pontoCima[2], float pontoMedio[2], float pontoBaixo[2]);
void pOriginal(int V[3], float P_[2], float P_1[2], float P_2[2], float P_3[2], float pontoOriginal[2][3]);
void zerarBuffer();
void zerarPhong();
void valoresPhong(float teste[3][3][10000]);
void coordenadaTela(float parametros[3][3][10000], float camera[3][14]);
void rasterizar(int V[3], float P1x, float P1y, float P2x, float P2y, float P3x, float P3y);
void desenhaLinha(int V[3], float P_1[2], float P_2[2], float P_3[2], float P1x, float P1y, float P2x, float P2y);
void desenha(float teste[3][3][10000]);

float obj[3][3][10000];
float camera[3][14];
float Phong[3][3][10000];
int zbuffer[800][450];

int main(void) {
    InitWindow(screenWidth, screenHeight, "Projeto Computação Gráfica");
    
    int arquivo = 10;

    carregaMalha(arquivo, obj);
    carregaCamera(camera);
    zerarBuffer(zbuffer);
    coordenadaTela(obj, camera);
    valoresPhong(obj);

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
                carregaCamera(camera);
                zerarBuffer(zbuffer);
                zerarPhong();
                coordenadaTela(obj, camera);
                valoresPhong(obj);
            }
        }
        if (IsKeyPressed('A')) {
            if (arquivo > 3) {
                arquivo--;
                carregaMalha(arquivo, obj);
                carregaCamera(camera);
                zerarBuffer();
                zerarPhong();
                coordenadaTela(obj, camera);
                valoresPhong(obj);
            }
        }
        if (IsKeyPressed('R')) {
            carregaMalha(arquivo, obj);
            carregaCamera(camera);
            zerarBuffer();
            zerarPhong();
            coordenadaTela(obj, camera);
            valoresPhong(obj);
        }

        BeginDrawing();  

            ClearBackground(BLACK);

            desenha(obj);
            zerarBuffer();

            DrawText("Aperte a ou d para navegar entre arquivos"  , 5, 2, 5, LIGHTGRAY);
            DrawText("Aperte r para aplicar a visão da camera"  , 5, 12, 5, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void desenha(float teste[3][3][10000]) {
    int numVertices = teste[0][0][0];
    float vertices[3][10000]; 
    for (int i = 0; i < numVertices; i++) {
        vertices[0][i] = teste[1][0][i];
        vertices[1][i] = teste[1][1][i];
        vertices[2][i] = teste[1][2][i];
    }

    int numTriangulos = teste[0][1][0];
    float triangulos[3][10000]; 
    for (int i = 0; i < numTriangulos; i++) {
        triangulos[0][i] = teste[2][0][i];
        triangulos[1][i] = teste[2][1][i];
        triangulos[2][i] = teste[2][2][i];
    }
    
    for (int i = 0; i < numTriangulos; i++) {
        // -1 pois armazena o valor na lista onde se encontra o vertice correspondentes
        int ver1 = triangulos[0][i] - 1;
        int ver2 = triangulos[1][i] - 1;
        int ver3 = triangulos[2][i] - 1;
        
        int V[3] = {ver1, ver2, ver3};
        rasterizar(V, vertices[0][ver1], vertices[1][ver1], vertices[0][ver2], vertices[1][ver2], vertices[0][ver3], vertices[1][ver3]);
    }
}

void desenhaLinha(int V[3], float P_1[2], float P_2[2], float P_3[2], float P1x, float P1y, float P2x, float P2y) {
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
        //encontrar o P original
        float ponto[2] = {auxP1x, auxP1y};
        float pontoOriginal[2][3];
        pOriginal(V, ponto, P_1, P_2, P_3, pontoOriginal);
        
        if(pontoOriginal[0][2] < zbuffer[(int)auxP1x][(int)auxP1y] && auxP1x >= 0 && auxP1x <= 800 && auxP1y >= 0 && auxP1y <= 450) {
            zbuffer[(int)auxP1x][(int)auxP1y] = (int)pontoOriginal[0][2];
            //encontrar o V
            float vetorV[3] = {(pontoOriginal[0][0])*(-1), (pontoOriginal[0][1])*(-1), (pontoOriginal[0][2])*(-1)};
            float _normalV_ = sqrt( pow(vetorV[0], 2) + pow(vetorV[1], 2) + pow(vetorV[2], 2) );
            vetorV[0] = (1/_normalV_) * vetorV[0];
            vetorV[1] = (1/_normalV_) * vetorV[1];
            vetorV[2] = (1/_normalV_) * vetorV[2];
            //encontrar o L
            float vetorL[3] = {camera[0][13] - pontoOriginal[0][0], camera[1][13] -pontoOriginal[0][1], camera[2][13] - pontoOriginal[0][2]};
            float _normalL_ = sqrt( pow(vetorL[0], 2) + pow(vetorL[1], 2) + pow(vetorL[2], 2) );
            vetorL[0] = (1/_normalL_) * vetorL[0];
            vetorL[1] = (1/_normalL_) * vetorL[1];
            vetorL[2] = (1/_normalL_) * vetorL[2];

            //calcular <N,L>
            float NL = (pontoOriginal[1][0]*vetorL[0]) + (pontoOriginal[1][1]*vetorL[1]) + (pontoOriginal[1][2]*vetorL[2]);
            //calcular <N,V>
            float NV = (pontoOriginal[1][0]*vetorV[0]) + (pontoOriginal[1][1]*vetorV[1]) + (pontoOriginal[1][2]*vetorV[2]);
            //encontrar o R
            //R = 2<N,L>N - L
            float vetorR[3];
            vetorR[0] = (NL*2*pontoOriginal[1][0]) - vetorL[0];
            vetorR[1] = (NL*2*pontoOriginal[1][1]) - vetorL[1];
            vetorR[2] = (NL*2*pontoOriginal[1][2]) - vetorL[2];
            //calcular <R,v>
            float RV = (vetorR[0]*vetorV[0]) + (vetorR[1]*vetorV[1]) + (vetorR[2]*vetorV[2]);

            //calcular iluminação
            //Ia = Ka * Iamb
            float Ia[3];
            Ia[0] = camera[0][6] * camera[0][8];
            Ia[1] = camera[1][6] * camera[0][8];
            Ia[2] = camera[2][6] * camera[0][8];
            //Id = <N,L> * Kd * Od * Il
            float Id[3];
            Id[0] = NL * camera[0][11] * camera[0][12] * camera[0][7];
            Id[1] = NL * camera[1][11] * camera[1][12] * camera[1][7];
            Id[2] = NL * camera[2][11] * camera[2][12] * camera[2][7];
            //Is = <R,v>^n * Ks * Il
            float Is[3];
            Is[0] = pow(RV, camera[0][10])* camera[0][9] * camera[0][7];
            Is[1] = pow(RV, camera[0][10])* camera[0][9] * camera[1][7];
            Is[2] = pow(RV, camera[0][10])* camera[0][9] * camera[2][7];

            //verificar casos especiais
            if(NL < 0) {
                if(NV < 0) {
                    pontoOriginal[1][0] = pontoOriginal[1][0]*(-1);
                    pontoOriginal[1][1] = pontoOriginal[1][1]*(-1);
                    pontoOriginal[1][2] = pontoOriginal[1][2]*(-1);

                    //calcular <N,V>
                    NL = (pontoOriginal[1][0]*vetorL[0]) + (pontoOriginal[1][1]*vetorL[1]) + (pontoOriginal[1][2]*vetorL[2]);
                    printf("NL: %f\n", NL);
                    //R = 2<N,L>N - L
                    vetorR[0] = (NL*2*pontoOriginal[1][0]) - vetorL[0];
                    vetorR[1] = (NL*2*pontoOriginal[1][1]) - vetorL[1];
                    vetorR[2] = (NL*2*pontoOriginal[1][2]) - vetorL[2];
                    printf("vetorR: %f %f %f\n", vetorR[0], vetorR[1], vetorR[2]);
                    //calcular <R,v>
                    RV = (vetorR[0]*vetorV[0]) + (vetorR[1]*vetorV[1]) + (vetorR[2]*vetorV[2]);
                    printf("RV: %f\n", RV);

                    //Id = <N,L> * Kd * Od * Il
                    Id[0] = NL * camera[0][11] * camera[0][12] * camera[0][7];
                    Id[1] = NL * camera[1][11] * camera[1][12] * camera[1][7];
                    Id[2] = NL * camera[2][11] * camera[2][12] * camera[2][7];
                    printf("Id: %f %f %f\n", Id[0], Id[1], Id[2]);
                    //Is = <R,v>^n * Ks * Il
                    Is[0] = pow(RV, camera[0][10])* camera[0][9] * camera[0][7];
                    Is[1] = pow(RV, camera[0][10])* camera[0][9] * camera[1][7];
                    Is[2] = pow(RV, camera[0][10])* camera[0][9] * camera[2][7];
                    printf("Is: %f %f %f\n", Is[0], Is[1], Is[2]);
                }else {
                    Id[0] = 0;
                    Id[1] = 0;
                    Id[2] = 0;

                    Is[0] = 0;
                    Is[1] = 0;
                    Is[2] = 0;
                }
            }
            if(RV < 0) {
                Is[0] = 0;
                Is[1] = 0;
                Is[2] = 0;
                printf("Is: %f %f %f\n", Is[0], Is[1], Is[2]);
            }

            //calcular cor
            //I = Ia + Id + Is
            float I[3] = {Ia[0] + Id[0] + Is[0], Ia[1] + Id[1] + Is[0], Ia[2] + Id[2] + Is[0]};
            printf("\nI: %f %f %f\n\n", I[0], I[1], I[2]);
            if(I[0] > 255) {
                I[0] = 255;
            }
            if(I[1] > 255) {
                I[1] = 255;
            }
            if(I[2] > 255) {
                I[2] = 255;
            }

            DrawPixel(auxP1x, auxP1y, (Color){I[0], I[1], I[2], 255});
        }

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

void rasterizar(int V[3], float P1x, float P1y, float P2x, float P2y, float P3x, float P3y) {
    float pontoCima[2] = {P1x, P1y};
    float pontoMedio[2] = {P2x, P2y};
    float pontoBaixo[2] = {P3x, P3y};

//Ordena os pontos do mais alto para o mais baixo
    ordena(pontoCima, pontoMedio, pontoBaixo);

    //printf("PontoCima X: %f PontoCima Y: %f\n", pontoCima[0], pontoCima[1]);
    //printf("pontoMedio X: %f pontoMedio Y: %f\n", pontoMedio[0], pontoMedio[1]);
    //printf("pontoBaixo X: %f pontoBaixo Y: %f\n\n", pontoBaixo[0], pontoBaixo[1]);

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
    
    //printf("aEsq: %f aDir: %f\n\n", aEsq, aDir);
    //printf("Xmin: %f Xmax: %f\n\n", Xmin, Xmax);


    if(aEsq == 0 && aDir == 0) {
        Xmax = pontoBaixo[0];
    }

    while (Yscan <= pontoMedio[1]) {
        float Xmin_ = (Xmin + aEsq);
        float Xmax_ = (Xmax + aDir);

        //printf("Xmin: %f Xmax: %f\n", round(Xmin), round(Xmax));
        //printf("Xmin_: %f Xmax_: %f\n\n", round(Xmin_), round(Xmax_));
        desenhaLinha(V, pontoCima, pontoMedio, pontoBaixo, round(Xmin), Yscan, round(Xmax), Yscan);

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
    //printf("pontoD X: %f pontoD Y: %f\n\n", pontoD[0], pontoD[1]);

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
    //printf("aEsq: %f aDir: %f\n", aEsq, aDir);
    if(pontoMedio[0] > pontoD[0]) {
        float troca = aEsq;
        aEsq = aDir;
        aDir = troca;
    }

    //printf("aEsq: %f aDir: %f\n\n", aEsq, aDir);
    //printf("Xmin: %f Xmax: %f\n\n", Xmin, Xmax);

    if(aEsq == 0 && aDir == 0) {
        Xmax = pontoMedio[0];
    }

    while (Yscan >= pontoMedio[1]) {
        float Xmin_ = (Xmin - aEsq);
        float Xmax_ = (Xmax - aDir);

        //printf("2Xmin: %f Xmax: %f\n", round(Xmin), round(Xmax));
        //printf("2Xmin_: %f Xmax_: %f\n\n", round(Xmin_), round(Xmax_));
        desenhaLinha(V, pontoCima, pontoMedio, pontoBaixo, round(Xmin), Yscan, round(Xmax), Yscan);

        Yscan--;
        Xmin = Xmin_;
        Xmax = Xmax_;
    }
}

void coordenadaTela(float parametros[3][3][10000], float camera[3][14]) {
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

        //calcular o U = N * Vortogonal
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
        Phong[0][0][i] = Pvista[0];
        Phong[0][1][i] = Pvista[1];
        Phong[0][2][i] = Pvista[2];

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

void valoresPhong(float teste[3][3][10000]) {
    int numVertices = teste[0][0][0];

    int numTriangulos = teste[0][1][0];
    float triangulos[3][10000]; 
    for (int i = 0; i < numTriangulos; i++) {
        triangulos[0][i] = teste[2][0][i];
        triangulos[1][i] = teste[2][1][i];
        triangulos[2][i] = teste[2][2][i];
    }

    for(int i = 0; i < numTriangulos; i++) {
        int ver1 = triangulos[0][i] - 1;
        int ver2 = triangulos[1][i] - 1;
        int ver3 = triangulos[2][i] - 1;

        float vet1[3] = {Phong[0][0][ver2] - Phong[0][0][ver1], Phong[0][1][ver2] - Phong[0][1][ver1], Phong[0][2][ver2] - Phong[0][2][ver1]};
        float vet2[3] = {Phong[0][0][ver3] - Phong[0][0][ver1], Phong[0][1][ver3] - Phong[0][1][ver1], Phong[0][2][ver3] - Phong[0][2][ver1]};

        //calcular a normal = vetor1 * vetor2
        float normal[3];
        normal[0] = (vet1[1]*vet2[2]) - (vet1[2]*vet2[1]);
        normal[1] = (vet2[0]*vet1[2]) - (vet2[2]*vet1[0]);
        normal[2] = (vet1[0]*vet2[1]) - (vet1[1]*vet2[0]);
    
        //normalizar
        float _normal_ = sqrt( pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2) );
        Phong[1][0][i] = (1/_normal_) * normal[0];
        Phong[1][1][i] = (1/_normal_) * normal[1];
        Phong[1][2][i] = (1/_normal_) * normal[2];
    }

    for (int i = 0; i < numVertices; i++) {
        for(int y = 0; y < numTriangulos; y++) {
            if(triangulos[0][y] == i + 1 || triangulos[1][y] == i + 1 || triangulos[2][y] == i + 1) {
                Phong[2][0][i] = Phong[1][0][y] + Phong[2][0][i];
                Phong[2][1][i] = Phong[1][1][y] + Phong[2][1][i];
                Phong[2][2][i] = Phong[1][2][y] + Phong[2][2][i];
            }
        }

        //normalizar
        float _normal_ = sqrt( pow(Phong[2][0][i], 2) + pow(Phong[2][1][i], 2) + pow(Phong[2][2][i], 2) );
        Phong[2][0][i] = (1/_normal_) * Phong[2][0][i];
        Phong[2][1][i] = (1/_normal_) * Phong[2][1][i];
        Phong[2][2][i] = (1/_normal_) * Phong[2][2][i];
    }
}

void zerarPhong() {
    for(int i = 0; i < 3; i++) {
        for(int f = 0; f < 3; f++) {
            for(int l = 0; l < 10000; l++) {
                Phong[i][f][l] = 0;
            }
        }
    }
}

void zerarBuffer() {
    for(int i = 0; i < 800; i++) {
        for(int f = 0; f < 450; f++) {
            zbuffer[i][f] = 999999;
        }
    }
}

void pOriginal(int V[3], float P_[2], float P_1[2], float P_2[2], float P_3[2], float pontoOriginal[2][3]) {
    //float P_1[2] = {401, 225};
    //float P_2[2] = {330, 225};
    //float P_3[2] = {367, 251};
    //float P_[2] = {367, 230};

    float AreaT = (((P_1[0] - P_2[0])*(P_1[1] - P_3[1])) - ((P_1[0] - P_3[0])*(P_1[1] - P_2[1])))/2;

    float AreaA = (((P_[0] - P_2[0])*(P_[1] - P_3[1])) - ((P_[0] - P_3[0])*(P_[1] - P_2[1])))/2;
    float AreaB = (((P_1[0] - P_[0])*(P_1[1] - P_3[1])) - ((P_1[0] - P_3[0])*(P_1[1] - P_[1])))/2;
    float AreaC = (((P_1[0] - P_2[0])*(P_1[1] - P_[1])) - ((P_1[0] - P_[0])*(P_1[1] - P_2[1])))/2;

    float a = AreaA/AreaT;
    float b = AreaB/AreaT;
    float c = AreaC/AreaT;

    float P1[3] = {Phong[0][0][V[0]], Phong[0][1][V[0]], Phong[0][2][V[0]]};
    float P2[3] = {Phong[0][0][V[1]], Phong[0][1][V[1]], Phong[0][2][V[1]]};
    float P3[3] = {Phong[0][0][V[2]], Phong[0][1][V[2]], Phong[0][2][V[2]]};

    float normalP1[3] = {Phong[2][0][V[0]], Phong[2][1][V[0]], Phong[2][2][V[0]]};
    float normalP2[3] = {Phong[2][0][V[1]], Phong[2][1][V[1]], Phong[2][2][V[1]]};
    float normalP3[3] = {Phong[2][0][V[2]], Phong[2][1][V[2]], Phong[2][2][V[2]]};

    //printf("%f\n", a);
    //printf("%f\n", b);
    //printf("%f\n\n", c);

    pontoOriginal[0][0] = (P1[0]*a + P2[0]*b + P3[0]*c);
    pontoOriginal[0][1] = (P1[1]*a + P2[1]*b + P3[1]*c);
    pontoOriginal[0][2] = (P1[2]*a + P2[2]*b + P3[2]*c);

    pontoOriginal[1][0] = (normalP1[0]*a + normalP2[0]*b + normalP3[0]*c);
    pontoOriginal[1][1] = (normalP1[1]*a + normalP2[1]*b + normalP3[1]*c);
    pontoOriginal[1][2] = (normalP1[2]*a + normalP2[2]*b + normalP3[2]*c);
    //printf("%f %f %f\n", P1[0], P1[1], P1[2]);
    //printf("%f %f %f\n", P2[0], P2[1], P2[2]);
    //printf("%f %f %f\n", P3[0], P3[1], P2[2]);
    //printf("Ponto: %f %f %f\n\n", (P1[0]*a + P2[0]*b + P3[0]*c), (P1[1]*a + P2[1]*b + P3[1]*c), (P1[2]*a + P2[2]*b + P3[2]*c));
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

