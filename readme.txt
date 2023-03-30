Projeto da cadeira de Computação grafica da UFRPE.

---------------------------------Especificações 1 entrega--------------------------------------------------------------------
Universidade Federal Rural de Pernambuco (UFRPE)
Departamento de Computação (DC)
Computação Gráfica Básica (06230)
1a VA - Especificação

Carregar na memória uma malha de triângulos referente a um objeto 3D armazenada em
arquivo de texto e desenhar seus vértices na tela. O arquivo utilizado para armazenar uma
malha com n vértices e k triângulos possui o seguinte formato:
<no de vértices> <no de triângulos>
<coordenada x do vértice 1> <coordenada y do vértice 1> <coordenada z do vértice 1>
<coordenada x do vértice 2> <coordenada y do vértice 2> <coordenada z do vértice 2>
...
<coordenada x do vértice n> <coordenada y do vértice n> <coordenada z do vértice n>
<índice do vértice 1 do triângulo 1> <índice do vértice 2 do triângulo 1> <índice do vértice 3 do triângulo 1>
<índice do vértice 1 do triângulo 2> <índice do vértice 2 do triângulo 2> <índice do vértice 3 do triângulo 2>
...
<índice do vértice 1 do triângulo k> <índice do vértice 2 do triângulo k> <índice do vértice 3 do triângulo k>
Exemplo de arquivo:
4 4
1 1 1
1 30 1
30 30 1
1 1 30
1 2 3
1 2 4
2 3 4
1 3 4

Uma vez que a malha foi carregada na memória, deve-se obter a projeção em perspectiva
de seus vértices.
A aplicação deverá carregar a partir de um arquivo de texto os seguintes parâmetros da
câmera virtual:
• Ponto C;
• Vetores N e V;
• Escalares d, hx e hy.

Exemplo de parâmetros de câmera:
N = 0 1 -1
V = 0 -1 -1
d = 5
hx = 2
hy = 2
C = 0 -500 500

O usuário deve ser capaz de alterar os valores dos parâmetros no arquivo de texto,
recarregá-los e redesenhar o objeto sem precisar fechar a aplicação e abri-la novamente
(ex: o usuário pode pressionar uma tecla específica para recarregar os parâmetros a partir
do arquivo de texto e redesenhar o objeto).
Deve-se converter os vértices do objeto de coordenadas mundiais para coordenadas de
vista, realizar a projeção em perspectiva, converter para coordenadas normalizadas e por
fim para coordenadas de tela. Após isso, deve-se utilizar o algoritmo de rasterização de
polígonos scan line conversion (varredura) para preencher os triângulos projetados. Os
pixels da tela correspondentes aos triângulos projetados e preenchidos devem ser pintados
de branco, enquanto que os demais pixels devem ser pintados de preto.
Qualquer linguagem de programação pode ser utilizada (C, Java, etc.). A única função
gráfica que pode ser utilizada é a que desenha um pixel colorido na tela. Apenas as
bibliotecas padrão da linguagem escolhida podem ser usadas.
Obs.: caso desejado, é permitido usar uma biblioteca externa que ofereça a função de pintar
um pixel colorido na tela.
-----------------------------------------------------------------------------------------------------

---------------------------------Especificações 2 entrega--------------------------------------------------------------------
Universidade Federal Rural de Pernambuco (UFRPE)
Departamento de Computação (DC)
Computação Gráfica Básica (06230)
2a VA - Especificação

Alterar o programa da 1a VA para renderizar um objeto 3D utilizando os modelos de
tonalização e iluminação de Phong.
A aplicação deverá carregar a partir de um arquivo de texto os seguintes parâmetros da
câmera virtual e de iluminação:
• Ponto C;
• Vetores N e V;
• Escalares d, hx e hy;
• Cores RGB Iamb e Il;
• Escalares Ka, Ks e η;
• Vetores Kd e Od;
• Ponto Pl.

Exemplo de parâmetros de iluminação:
Iamb = 100 100 100
Ka = 0.2
Il = 127 213 254
Pl = 60 5 -10
Kd = 0.5 0.3 0.2
Od = 0.7 0.5 0.8
Ks = 0.5
η = 1
O usuário deve ser capaz de alterar os valores dos parâmetros no arquivo de texto,
recarregá-los e redesenhar o objeto sem precisar fechar a aplicação e abri-la novamente
(ex: o usuário pode pressionar uma tecla específica para recarregar os parâmetros a partir
do arquivo de texto e redesenhar o objeto).
Deve-se utilizar o algoritmo de visibilidade z-buffer e o algoritmo de rasterização de
polígonos scan line conversion (varredura).
Qualquer linguagem de programação pode ser utilizada (C, Java, etc.). A única função
gráfica que pode ser utilizada é a que desenha um pixel colorido na tela. Apenas as
bibliotecas padrão da linguagem escolhida podem ser usadas.
Obs.: caso desejado, é permitido usar uma biblioteca externa que ofereça a função de pintar
um pixel colorido na tela.
