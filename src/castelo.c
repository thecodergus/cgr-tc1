#include "castelo.h"

// Variáveis estáticas para controle de rotação e posição
static double x1 = 0, rotX = 0.0, rotY = 0.0, rot = 0;
int wire = 0, panaroma = 0, open = 0, walk = 0;
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0;

// Função para tratamento das teclas especiais
void SpecialKey(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:  // Seta para cima
      rotX -= 0.1;     // Diminui a rotação em X
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:  // Seta para baixo
      rotX += 0.1;       // Aumenta a rotação em X
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:  // Seta para esquerda
      rotY -= 0.1;       // Diminui a rotação em Y
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:  // Seta para direita
      rotY += 0.1;        // Aumenta a rotação em Y
      glutPostRedisplay();
      break;
  }
}

// Função para desenhar a bandeira
void flag() {
  glPushMatrix();
  glTranslated(3.9, 6, -4.25);  // Posiciona a bandeira no mundo 3D
  glScaled(8, 5, 0.5);          // Define a escala da bandeira

  if (wire)
    glutWireCube(0.1);  // Desenha a bandeira em modo "arame" (wireframe)
  else
    glutSolidCube(0.1);  // Desenha a bandeira em modo sólido

  glPopMatrix();
}

// Função para desenhar a caixa central e os seus blocos laterais
void box() {
  glPushMatrix();
  glScaled(2, 0.5, 2);  // Define a escala da caixa central

  if (wire)
    glutWireCube(1);  // Desenha a caixa central em modo "arame" (wireframe)
  else
    glutSolidCube(1);  // Desenha a caixa central em modo sólido

  glPopMatrix();

  // Desenha o bloco superior esquerdo
  glPushMatrix();
  glTranslated(-0.75, 0.5, 0.75);
  if (wire)
    glutWireCube(0.5);
  else
    glutSolidCube(0.5);
  glPopMatrix();

  // Desenha o bloco superior direito
  glPushMatrix();
  glTranslated(0.75, 0.5, 0.75);
  if (wire)
    glutWireCube(0.5);
  else
    glutSolidCube(0.5);
  glPopMatrix();

  // Desenha o bloco inferior esquerdo
  glPushMatrix();
  glTranslated(-0.75, 0.5, -0.75);
  if (wire)
    glutWireCube(0.5);
  else
    glutSolidCube(0.5);
  glPopMatrix();

  // Desenha o bloco inferior direito
  glPushMatrix();
  glTranslated(0.75, 0.5, -0.75);
  if (wire)
    glutWireCube(0.5);
  else
    glutSolidCube(0.5);
  glPopMatrix();
}

// Função para desenhar um cilindro entre dois pontos com um determinado raio e número de subdivisões
void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions, GLUquadricObj *quadric) {
  float vx = x2 - x1;
  float vy = y2 - y1;
  float vz = z2 - z1;

  // Caso degenerado: quando z1 == z2, usa uma aproximação para evitar divisão por zero
  if (vz == 0) vz = .0001;

  float v = sqrt(vx * vx + vy * vy + vz * vz);  // Calcula o comprimento do vetor
  float ax = 57.2957795 * acos(vz / v);         // Calcula o ângulo de rotação em torno do eixo Y
  if (vz < 0.0) ax = -ax;                       // Corrige o sinal do ângulo, se necessário
  float rx = -vy * vz;                          // Componente X do eixo de rotação
  float ry = vx * vz;                           // Componente Y do eixo de rotação

  glPushMatrix();

  // Desenha o corpo do cilindro
  glTranslatef(x1, y1, z1);                                  // Move para a posição inicial do cilindro
  glRotatef(ax, rx, ry, 0.0);                                // Rotação em torno do eixo calculado
  gluQuadricOrientation(quadric, GLU_OUTSIDE);               // Define a orientação da superfície
  gluCylinder(quadric, radius, radius, v, subdivisions, 1);  // Desenha o cilindro

  // Desenha a primeira base do cilindro
  gluQuadricOrientation(quadric, GLU_INSIDE);      // Define a orientação da superfície
  gluDisk(quadric, 0.0, radius, subdivisions, 1);  // Desenha a base
  glTranslatef(0, 0, v);                           // Move para a posição da segunda base

  // Desenha a segunda base do cilindro
  gluQuadricOrientation(quadric, GLU_OUTSIDE);     // Define a orientação da superfície
  gluDisk(quadric, 0.0, radius, subdivisions, 1);  // Desenha a base

  glPopMatrix();
}

// Função conveniente para desenhar um cilindro entre dois pontos com um determinado raio e número de subdivisões
void renderCylinder_convenient(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions) {
  // Cria um objeto quadric para desenhar o cilindro
  GLUquadricObj *quadric = gluNewQuadric();
  gluQuadricNormals(quadric, GLU_SMOOTH);  // Define as normais suavizadas para o cilindro

  // Chama a função de desenho do cilindro
  renderCylinder(x1, y1, z1, x2, y2, z2, radius, subdivisions, quadric);

  // Libera a memória ocupada pelo objeto quadric
  gluDeleteQuadric(quadric);
}

void castle(double rang) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Limpa o buffer de cor e profundidade
  glLoadIdentity();                                    // Carrega a matriz identidade
  if (walk) {                                          // Se walk for verdadeiro
    glRotatef(xrot, 1.0, 0.0, 0.0);                    // Rotação da câmera em xrot graus em torno do eixo X
    glRotatef(yrot, 0.0, 1.0, 0.0);                    // Rotação da câmera em yrot graus em torno do eixo Y
    glTranslated(-xpos, -ypos, -zpos);                 // Translação da câmera para a posição (-xpos, -ypos, -zpos)
  } else if (panaroma)
    gluLookAt(0.5, 15, 1, 0, 9, -7, 0, 1, 0);  // Define a posição da câmera e o ponto para o qual ela está olhando
  if (!walk)
    glTranslatef(0.0f, -5.0f, -48.0f);  // Translação do objeto para a posição (0.0f, -5.0f, -48.0f)
  else {
    glTranslatef(0, 1, -75);  // Translação do objeto para a posição (0, 1, -75)
    glScaled(2, 2, 2.5);      // Escala o objeto em 2 em x, 2 em y e 2.5 em z
  }
  glRotatef(rang, 0.0f, 1.0f, 0.0f);  // Rotação do objeto em rang graus em torno do eixo Y
  glPushMatrix();                     // Salva a matriz atual na pilha
  glTranslated(0, 0, -10);            // Translação do objeto para a posição (0, 0, -10)
  glScaled(1, 1, 1.3);                // Escala o objeto em 1 em x, 1 em y e 1.3 em z

  glColor3f(210 / 255.0, 180 / 255.0, 140 / 255.0);  // Define a cor do objeto

  // Desenha o cilindro esquerdo da frente
  glPushMatrix();
  glTranslatef(-22.5, -3.0, 23);
  renderCylinder_convenient(0, 0, 0, 0, 5, 0, 1.4, 20);
  glPopMatrix();

  // Desenha o cilindro direito da frente
  glPushMatrix();
  glTranslatef(22.5, -3.0, 23);
  renderCylinder_convenient(0, 0, 0, 0, 5, 0, 1.4, 20);
  glPopMatrix();

  // Desenha o cilindro esquerdo de trás
  glPushMatrix();
  glTranslatef(-22.5, -3.0, -8.0);
  renderCylinder_convenient(0, 0, 0, 0, 5, 0, 1.4, 20);
  glPopMatrix();

  // Desenha o cilindro direito de trás
  glPushMatrix();
  glTranslatef(22.5, -3.0, -8.0);
  renderCylinder_convenient(0, 0, 0, 0, 5, 0, 1.4, 20);
  glPopMatrix();

  // Desenha a parede esquerda
  glPushMatrix();                // salva a matriz de transformação atual na pilha
  glScaled(0.3, 1.5, 15);        // escala a parede em x, y e z
  glTranslatef(0.0, 0.0, -0.5);  // move a parede para trás em relação à origem
  glTranslatef(-75, -1, 1.0);    // move a parede para a esquerda e para baixo em relação à origem
  if (wire)                      // se wireframe estiver ativado
    glutWireCube(2);             // desenha a parede como um cubo vazado
  else                           // senão
    glutSolidCube(2);            // desenha a parede como um cubo sólido
  glPopMatrix();                 // restaura a matriz de transformação anterior da pilha

  // Desenha a parede direita
  glPushMatrix();                // salva a matriz de transformação atual na pilha
  glScaled(0.3, 1.5, 15);        // escala a parede em x, y e z
  glTranslatef(0.0, 0.0, -0.5);  // move a parede para trás em relação à origem
  glTranslatef(75, -1, 1.0);     // move a parede para a direita e para baixo em relação à origem
  if (wire)                      // se wireframe estiver ativado
    glutWireCube(2);             // desenha a parede como um cubo vazado
  else                           // senão
    glutSolidCube(2);            // desenha a parede como um cubo sólido
  glPopMatrix();                 // restaura a matriz de transformação anterior da pilha

  // Desenha a parede de fundo
  glPushMatrix();                 // salva a matriz de transformação atual na pilha
  glRotatef(90, 0, 1, 0);         // roda a parede 90 graus em torno do eixo y
  glScaled(0.3, 1.5, 23);         // escala a parede em x, y e z
  glTranslatef(0.0, 0.0, -0.5);   // move a parede para trás em relação à origem
  glTranslatef(26.5, -1.0, 0.5);  // move a parede para a direita e para baixo em relação à origem
  if (wire)                       // se wireframe estiver ativado
    glutWireCube(2);              // desenha a parede como um cubo vazado
  else                            // senão
    glutSolidCube(2);             // desenha a parede como um cubo sólido
  glPopMatrix();                  // restaura a matriz de transformação anterior da pilha

  // Desenha o chão
  glColor3f(0 / 255.0, 255 / 255.0, 0 / 255.0);  // define a cor do chão como verde
  glPushMatrix();                                // salva a matriz de transformação atual na pilha
  glTranslated(0, -3.5, 8);                      // move o chão para baixo e para frente em relação à origem
  glScaled(60, 0.5, 44);                         // escala o chão em x, y e z
  if (wire)                                      // se wireframe estiver ativado
    glutWireCube(1);                             // desenha o chão como um cubo vazado
  else                                           // senão
    glutSolidCube(1);                            // desenha o chão como um cubo sólido
  glPopMatrix();                                 // restaura a matriz de transformação anterior da pilha

  glColor3f(210 / 255.0, 180 / 255.0, 140 / 255.0);  // define a cor das paredes como marrom claro

  // Desenha a parede da esquerda do portão
  glPushMatrix();                // salva a matriz atual na pilha
  glScaled(9, 1.5, 0.5);         // escala a parede da esquerda do portão
  glTranslatef(-1.5, -1, 47.0);  // move a parede da esquerda do portão para a posição desejada
  if (wire)                      // se wireframe estiver ativado
    glutWireCube(2);             // desenha a parede da esquerda do portão em wireframe
  else                           // senão
    glutSolidCube(2);            // desenha a parede da esquerda do portão sólida
  glPopMatrix();                 // restaura a matriz anterior da pilha

  // Desenha a parede da direita do portão
  glPushMatrix();               // salva a matriz atual na pilha
  glScaled(9, 1.5, 0.5);        // escala a parede da direita do portão
  glTranslatef(1.5, -1, 47.0);  // move a parede da direita do portão para a posição desejada
  if (wire)                     // se wireframe estiver ativado
    glutWireCube(2);            // desenha a parede da direita do portão em wireframe
  else                          // senão
    glutSolidCube(2);           // desenha a parede da direita do portão sólida
  glPopMatrix();                // restaura a matriz anterior da pilha

  // Desenha a parte esquerda do portão
  glPushMatrix();                // salva a matriz atual na pilha
  glScaled(1.2, 2.6, 0.9);       // escala a entrada da esquerda
  glTranslatef(-4.6, -0.2, 26);  // move a entrada da esquerda para a posição desejada
  if (wire)                      // se wireframe estiver ativado
    glutWireCube(2);             // desenha a entrada da esquerda em wireframe
  else                           // senão
    glutSolidCube(2);            // desenha a entrada da esquerda sólida
  glPopMatrix();                 // restaura a matriz anterior da pilha

  // Desenha a parte direita do portão
  glPushMatrix();               // salva a matriz atual na pilha
  glScaled(1.2, 2.6, 0.9);      // escala a entrada da direita
  glTranslatef(4.6, -0.2, 26);  // move a entrada da direita para a posição desejada
  if (wire)                     // se wireframe estiver ativado
    glutWireCube(2);            // desenha a entrada da direita em wireframe
  else                          // senão
    glutSolidCube(2);           // desenha a entrada da direita sólida
  glPopMatrix();                // restaura a matriz anterior da pilha

  // Desenha a entrada t1
  glPushMatrix();
  // Aplica uma escala de 4.4 na direção x, 1 na direção y e 0.9 na direção z
  glScaled(4.4, 1, 0.9);
  // Move o objeto para a posição (0, 2, 26)
  glTranslatef(0, 2, 26);
  // Se wire for verdadeiro, desenha um cubo em modo de arame
  if (wire) glutWireCube(2);
  // Senão, desenha um cubo sólido
  else
    glutSolidCube(2);
  // Restaura a matriz de modelagem para o estado anterior
  glPopMatrix();

  // Entrada t2
  glPushMatrix();
  // Aplica uma escala de 4.4 na direção x, 1 na direção y e 0.9 na direção z
  glScaled(4.4, 1, 0.9);
  // Move o objeto para a posição (0, 3, 26)
  glTranslatef(0, 3, 26);
  // Se wire for verdadeiro, desenha um cubo em modo de arame com tamanho 1.5
  if (wire) glutWireCube(1.5);
  // Senão, desenha um cubo sólido com tamanho 1.5
  else
    glutSolidCube(1.5);
  // Restaura a matriz de modelagem para o estado anterior
  glPopMatrix();

  // Entrada t2
  glPushMatrix();
  // Aplica uma escala de 4.4 na direção x, 1 na direção y e 0.9 na direção z
  glScaled(4.4, 1, 0.9);
  // Move o objeto para a posição (0, 4, 26)
  glTranslatef(0, 4, 26);
  // Se wire for verdadeiro, desenha um cubo em modo de arame com tamanho 1.0
  if (wire) glutWireCube(1.0);
  // Senão, desenha um cubo sólido com tamanho 1.0
  else
    glutSolidCube(1.0);
  // Restaura a matriz de modelagem para o estado anterior
  glPopMatrix();

  // Define a cor do objeto como (230, 232, 250)
  glColor3d(230, 232, 250);

  // Audi 1
  glPushMatrix();
  // Move o objeto para a posição (0, 4.78, 23.5)
  glTranslated(0, 4.78, 23.5);
  // Se wire for verdadeiro, desenha um toro em modo de arame com raio interno 0.08 e raio externo 0.3
  if (wire) glutWireTorus(0.08, 0.3, 10, 20);
  // Senão, desenha um toro sólido com raio interno 0.08 e raio externo 0.3
  else
    glutSolidTorus(0.08, 0.3, 10, 20);
  // Restaura a matriz de modelagem para o estado anterior
  glPopMatrix();

  // Audi 2
  glPushMatrix();
  // Move o objeto para a posição (0.5, 4.78, 23.5)
  glTranslated(0.5, 4.78, 23.5);
  // Se wire for verdadeiro, desenha um toro em modo de arame com raio interno 0.08 e raio externo 0.3
  if (wire) glutWireTorus(0.08, 0.3, 10, 20);
  // Senão, desenha um toro sólido com raio interno 0.08 e raio externo 0.3
  else
    glutSolidTorus(0.08, 0.3, 10, 20);
  // Restaura a matriz de modelagem para o estado anterior
  glPopMatrix();

  // Audi 3
  glPushMatrix();
  // Move o objeto para a posição (-0.5, 4.78, 23.5)
  glTranslated(-0.5, 4.78, 23.5);
  // Se wire for verdadeiro, desenha um toro em modo de arame com raio interno 0.08 e raio externo 0.3
  if (wire) glutWireTorus(0.08, 0.3, 10, 20);
  // Senão, desenha um toro sólido com raio interno 0.08 e raio externo 0.3
  else
    glutSolidTorus(0.08, 0.3, 10, 20);
  // Restaura a matriz de modelagem para o estado anterior
  glPopMatrix();

  // Define a cor do objeto como (139/255, 69/255, 19/255)
  glColor3f(139 / 255.0, 69 / 255.0, 19 / 255.0);

  // Desenha o portão
  glPushMatrix();
  // Se open for verdadeiro, rotaciona o objeto -10 graus em torno do eixo y
  if (open) glRotatef(-10, 0, 1, 0);
  // Aplica uma escala de 4.2 na direção x, 5 na direção y e 0.5 na direção z
  glScaled(4.2, 5, 0.5);
  // Move o objeto para a posição (-0.5, -0.2, 46)
  glTranslatef(-0.5, -0.2, 46);
  // Se wire for verdadeiro, desenha um cubo em modo de arame
  if (wire) glutWireCube(1.0);
  // Senão, desenha um cubo sólido
  else
    glutSolidCube(1.0);
  // Restaura a matriz de modelagem para o estado anterior
  glPopMatrix();

  // Desenha o portão
  glPushMatrix();
  // Se open for verdadeiro, rotaciona o objeto 10 graus em torno do eixo y
  if (open) glRotatef(10, 0, 1, 0);
  // Aplica uma escala de 4.2 na direção x, 5 na direção y e 0.5 na direção z
  glScaled(4.2, 5, 0.5);
  // Move o objeto para a posição (0.5, -0.2, 46)
  glTranslatef(0.5, -0.2, 46);
  // Se wire for verdadeiro, desenha um cubo em modo de arame
  if (wire) glutWireCube(1.0);
  // Senão, desenha um cubo sólido
  else
    glutSolidCube(1.0);
  // Restaura a matriz de modelagem para o estado anterior
  glPopMatrix();

  // Restaura a matriz de modelagem para o estado anterior (não há um correspondente glPushMatrix())
  glPopMatrix();

  // Move o objeto para a posição (0.5, -1.5, -6)
  glTranslated(0.5, -1.5, -6);

  // Restaura a matriz de modelagem para o estado anterior (não há um correspondente glPushMatrix())
  glPopMatrix();

  // Desenha o cubo principal

  // Desenha a parede frontal
  glColor3f(255 / 255.0, 185 / 255.0, 15 / 255.0);  // Define a cor para a parede frontal
  glPushMatrix();                                   // Salva a matriz atual na pilha
  glTranslated(-7.5, -1, 3);                        // Move a matriz para a posição da parede frontal
  glScaled(7, 3, 0.8);                              // Aplica escala na matriz para definir o tamanho da parede frontal
  if (wire)                                         // Se wireframe estiver ativado
    glutWireCube(1.0);                              // Desenha um cubo em wireframe
  else                                              // Senão
    glutSolidCube(1.0);                             // Desenha um cubo sólido
  glPopMatrix();                                    // Restaura a matriz anterior da pilha

  // Desenha a parede lateral
  glPushMatrix();                // Salva a matriz atual na pilha
  glTranslated(-10.6, -1, 1.3);  // Move a matriz para a posição da parede lateral
  glRotated(-90, 0, 1, 0);       // Rotação aplicada na matriz para orientar a parede lateral
  glScaled(3, 3, 0.8);           // Aplica escala na matriz para definir o tamanho da parede lateral
  if (wire)                      // Se wireframe estiver ativado
    glutWireCube(1.0);           // Desenha um cubo em wireframe
  else                           // Senão
    glutSolidCube(1.0);          // Desenha um cubo sólido
  glPopMatrix();                 // Restaura a matriz anterior da pilha

  // Desenha a parede traseira
  glPushMatrix();                // Salva a matriz atual na pilha
  glTranslated(-6.2, -1, -3.6);  // Move a matriz para a posição da parede traseira
  glScaled(6, 3, 0.8);           // Aplica escala na matriz para definir o tamanho da parede traseira
  if (wire)                      // Se wireframe estiver ativado
    glutWireCube(1.0);           // Desenha um cubo em wireframe
  else                           // Senão
    glutSolidCube(1.0);          // Desenha um cubo sólido
  glPopMatrix();                 // Restaura a matriz anterior da pilha

  // Define a cor para as próximas operações de desenho
  glColor3f(148 / 255.0, 0 / 255.0, 211 / 255.0);

  // Desenha o cilindro grande
  glPushMatrix();                                          // Salva a matriz atual na pilha
  glTranslated(-10.3, -2.5, -2.2);                         // Translada a matriz para a posição do cilindro
  renderCylinder_convenient(0, 0, 0, 0, 4.4, 0, 2.2, 20);  // Desenha o cilindro com as especificações dadas
  glPopMatrix();                                           // Restaura a matriz anterior

  glColor3f(255 / 255.0, 0 / 255.0, 0 / 255.0);  // Define a cor para vermelho

  // Desenha o cone grande
  glPushMatrix();                   // Salva a matriz atual na pilha
  glRotatef(-90, 1, 0, 0);          // Rotação em torno do eixo y
  glTranslated(-10.3, 2, 1.6);      // Translada a matriz para a posição do cone
  if (wire)                         // Se wireframe estiver ativado
    glutWireCone(3.4, 3, 15, 15);   // Desenha o cone em wireframe
  else                              // Senão
    glutSolidCone(3.4, 3, 15, 15);  // Desenha o cone sólido
  glPopMatrix();                    // Restaura a matriz anterior

  // Desenha o cubo
  glColor3f(255 / 255.0, 185 / 255.0, 15 / 255.0);  // Define a cor para o cubo
  glPushMatrix();                                   // Salva a matriz atual na pilha
  glTranslated(7.6, -1, -1);                        // Translada a matriz para a posição do cubo
  glScaled(2.2, 1, 1.5);                            // Aplica escala no cubo
  if (wire)                                         // Se wireframe estiver ativado
    glutWireCube(4.0);                              // Desenha o cubo em wireframe
  else                                              // Senão
    glutSolidCube(4.0);                             // Desenha o cubo sólido
  glPopMatrix();                                    // Restaura a matriz anterior

  glColor3f(238 / 255.0, 44 / 255.0, 44 / 255.0);  // Define a cor para a esfera

  // Desenha a esfera
  glPushMatrix();  // Salva a matriz atual na pilha

  // Desenha uma esfera sólida ou em arame, dependendo do valor da variável "wire"
  glTranslated(8, 1.3, -1);  // Move a origem para a posição (8, 1.3, -1)
  if (wire)                  // Se wire for verdadeiro, desenha a esfera em arame
    glutWireSphere(2.5, 15, 15);
  else  // Senão, desenha a esfera sólida
    glutSolidSphere(2.5, 15, 15);
  glPopMatrix();  // Restaura a matriz de transformação anterior

  // Define a cor como amarelo
  glColor3f(255 / 255.0, 185 / 255.0, 15 / 255.0);

  // Desenha um cubo sólido ou em arame, dependendo do valor da variável "wire"
  glPushMatrix();                // Salva a matriz de transformação atual
  glTranslatef(0.0, 0.0, -0.5);  // Move a origem para a posição (0, 0, -0.5)
  glScaled(1.6, 1.2, 2);         // Escala o cubo por um fator de 1.6 na direção x, 1.2 na direção y e 2 na direção z
  if (wire)                      // Se wire for verdadeiro, desenha o cubo em arame
    glutWireCube(4);
  else  // Senão, desenha o cubo sólido
    glutSolidCube(4);
  glPopMatrix();  // Restaura a matriz de transformação anterior

  // Define a cor como roxo
  glColor3f(0.576471, 0.439216, 0.858824);
  // Desenhar cilindro fr
  glPushMatrix();
  glTranslated(2.9, -2.45, 3.0);
  // Renderiza o cilindro com as dimensões e cor especificadas
  renderCylinder_convenient(0, 0, 0, 0, 6.05, 0, 0.7, 20);
  glPopMatrix();

  // Desenhar cilindro fl
  glPushMatrix();
  glTranslated(-3.6, -2.4, 3.0);
  // Renderiza o cilindro com as dimensões e cor especificadas
  renderCylinder_convenient(0, 0, 0, 0, 5.2, 0, 1.3, 20);
  glPopMatrix();

  // Desenhar cilindro br
  glPushMatrix();
  glTranslated(2.9, -2.4, -5.0);
  // Renderiza o cilindro com as dimensões e cor especificadas
  renderCylinder_convenient(0, 0, 0, 0, 10, 0, 1.5, 20);
  glPopMatrix();

  // Desenhar cilindro bm
  glPushMatrix();
  glTranslated(-1.2, -2.45, -4.1);
  // Renderiza o cilindro com as dimensões e cor especificadas
  renderCylinder_convenient(0, 0, 0, 0, 8, 0, 1.2, 50);
  glPopMatrix();

  // Define a cor para vermelho
  glColor3f(1, 0, 0);

  // Desenha o cone1
  glPushMatrix();
  // Rotação do cone1
  glRotatef(-90, 1, 0, 0);
  // Translação do cone1
  glTranslated(-1.2, 4.1, 4.6);
  // Condição para desenhar o cone1 em fio ou sólido
  if (wire)
    glutWireCone(1.8, 3.5, 15, 15);
  else
    glutSolidCone(1.8, 3.5, 15, 15);
  glPopMatrix();

  // Desenha o cone2
  glPushMatrix();
  // Rotação do cone2
  glRotatef(-90, 1, 0, 0);
  // Translação do cone2
  glTranslated(2.8, 4.8, 6.4);
  // Condição para desenhar o cone2 em fio ou sólido
  if (wire)
    glutWireCone(2.5, 5, 15, 15);
  else
    glutSolidCone(2.5, 5, 15, 15);
  glPopMatrix();

  // Escala e translada a caixa
  glScaled(1.8, 1.6, 1.8);
  glTranslated(-1.9, 2, 1.6);
  // Define a cor da caixa
  glColor3f(238 / 255.0, 44 / 255.0, 44 / 255.0);
  // Desenha a caixa
  box();
  // Define a cor do toro
  glColor3d(255, 215, 0);
  glPushMatrix();
  // Translação e rotação do toro
  glTranslated(3.5, 0.8, 0);
  glRotated(rot, 1, 1, 1);
  // Condição para desenhar o toro em fio ou sólido
  if (wire)
    glutWireTorus(0.15, 0.4, 10, 20);
  else
    glutSolidTorus(0.15, 0.4, 10, 20);
  glPopMatrix();

  // Define a cor do segundo toro
  glColor3d(230, 232, 250);
  glPushMatrix();
  // Translação e rotação do segundo toro
  glTranslated(3.5, 1.2, 0);
  glRotated(rot, 0, 0, 0);
  // Condição para desenhar o segundo toro em fio ou sólido
  if (wire)
    glutWireTorus(0.05, 0.5, 10, 20);
  else
    glutSolidTorus(0.05, 0.5, 10, 20);
  glPopMatrix();
  // Define a cor do cilindro
  glColor3f(0.439216, 0.858824, 0.576471);
  glPushMatrix();
  // Translação do cilindro
  glTranslated(3.45, 4.5, -4.25);
  // Desenha o cilindro
  renderCylinder_convenient(0, 0, 0, 0, 2, 0, 0.03, 20);
  glPopMatrix();

  // Define a cor da caixa
  glColor3f(166 / 255.0, 128 / 255.0, 100 / 255.0);

  // Desenha a primeira caixa
  glPushMatrix();
  glTranslated(-11, 0.4, -9.6);
  box();
  glPopMatrix();

  // Desenha a segunda caixa
  glPushMatrix();
  glTranslated(14.2, 0.4, 12.75);
  box();
  glPopMatrix();

  // Desenha a terceira caixa
  glPushMatrix();
  glTranslated(-11, 0.4, 12.75);
  box();
  glPopMatrix();

  // Desenha a quarta caixa
  glPushMatrix();
  glTranslated(14.2, 0.4, -9.6);
  box();
  glPopMatrix();

  // Define a cor da bandeira
  glColor3f(1, 0, 0);

  // Desenha a bandeira e atualiza a rotação
  flag();
  rot += 1;

  // Desenha os objetos na tela
  glFlush();
  glutSwapBuffers();
}

/**
 * Função responsável por incrementar o valor de x1 em 0.05 e chamar a função castle(x1).
 */
void inc() {
  x1 += 0.05;
  castle(x1);
}

/**
 * Função responsável por inicializar as configurações iniciais do OpenGL.
 */
void init() {
  // Define a cor de fundo da janela (azul claro)
  glClearColor(0.690196078, 0.87843137, 0.90196078, 1.0);

  // Define a vista da janela (tamanho da janela em pixels)
  glViewport(0, 0, 1050, 680);

  // Define a matriz de projeção ativa
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Define a projeção perspectiva com ângulo de visão de 60 graus, proporção da janela de 1050/680, plano próximo a 0,2 e plano distante a
  // 200
  gluPerspective(60.0f, (GLfloat)1050 / (GLfloat)680, 0.2f, 200.0f);

  // Define a matriz de modelagem ativa
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Define a profundidade de limpeza da tela
  glClearDepth(2.0f);

  // Habilita o teste de profundidade para ocultar as faces traseiras dos objetos
  glEnable(GL_DEPTH_TEST);

  // Define a função de comparação de profundidade para teste de profundidade
  glDepthFunc(GL_LEQUAL);
}

/**
 * Função responsável por exibir o objeto na tela.
 */
void displayCastelo() {
  // Define as propriedades do material da superfície do objeto
  GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};   // cor ambiente
  GLfloat mat_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};   // cor difusa
  GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};  // cor especular
  GLfloat mat_shininess[] = {100.0f};                 // brilho

  // Aplica as propriedades do material à face frontal do objeto
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  // Define a intensidade e a posição da luz
  GLfloat lightIntensity[] = {0.3f, 0.3f, 0.3f, 1.0f};
  GLfloat light_position[] = {2.0f, 5.0f, 6.0f, 0.0f};

  // Aplica a intensidade e a posição à luz 0
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

  // Desenha os objetos na tela
  glFlush();
  glutSwapBuffers();

  // Chama a função que desenha a fortaleza com a posição x1
  castle(x1);
}

/**
 * Função responsável por lidar com eventos de teclado relacionados ao castelo.
 *
 * @param key A tecla pressionada.
 * @param x A posição x do cursor na janela.
 * @param y A posição y do cursor na janela.
 */
void keyboardCastelo(unsigned char key, int x, int y) {

  switch (key)
  {
  case 'q':
      // movimento para cima
      xrot += 1;
      if (xrot > 360) xrot -= 360;
      break;
  case 'z':
    // movimento para baixo
    xrot -= 1;
    if (xrot < -360)
      xrot += 360;
    break;
  case 'w':
    // movimento para frente
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos += (float)sin(yrotrad);
    zpos -= (float)cos(yrotrad);
    ypos -= (float)sin(xrotrad);
    break;
  case 's':
    // movimento para trás
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos -= (float)sin(yrotrad);
    zpos += (float)cos(yrotrad);
    ypos += (float)sin(xrotrad);
    break;
  case 'd':
    // movimento para esquerda
    yrot += 1;
    if (yrot > 360)
      yrot -= 360;
    break;

  case 'a':
    // movimento para direita
    yrot -= 1;
    if (yrot < -360)
      yrot += 360;
    break;

  case 27:
    // saída do programa (tecla ESC)
    exit(0);
    break;

  case 'o':
    // alternar estado do portao de "open"
    open = !open;
    break;

      default:
    break;
  }
  displayCastelo();              // exibe o resultado
}

/**
 * Função responsável por executar a ação correspondente ao item selecionado no menu.
 *
 * @param id O identificador do item selecionado.
 */
void menuCastelo(int id) {
  // switch para selecionar a opção do menu
  switch (id) {
    case 1:
      glutIdleFunc(inc);  // ativa animação
      break;
    case 2:
      panaroma = 0;  // desativa modo panorama
      break;
    case 3:
      panaroma = 1;  // ativa modo panorama
      break;
    case 4:
      wire = 1;  // ativa modo wireframe (malha de arame)
      break;
    case 5:
      wire = 0;  // desativa modo wireframe
      break;
    case 6:
      walk = !walk;  // alterna o estado da variável "walk"
      break;
    case 7:
      exit(0);  // sai do programa
      break;
  }
  glFlush();            // limpa o buffer de comandos OpenGL
  glutSwapBuffers();    // troca os buffers da tela
  glutPostRedisplay();  // solicita a reexibição da janela
}

/**
 * Função responsável por abrir a janela do castelo.
 * Inicializa o modo de exibição da janela, define o tamanho e a posição da janela,
 * cria a janela com o título "fort", define as funções de callback para redesenhar a janela,
 * capturar teclas normais e teclas especiais, habilita a iluminação, define o modelo de sombreamento,
 * habilita o teste de profundidade, habilita a normalização dos vetores normais,
 * habilita o uso da cor do material, cria um menu com opções e atribui o menu ao botão direito do mouse.
 * Por fim, inicializa parâmetros e variáveis e inicia o loop principal da aplicação.
 */
void abrirCastelo() {
  // inicializa o modo de exibição da janela
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // define o tamanho da janela
  glutInitWindowSize(950, 700);

  // define a posição da janela na tela
  glutInitWindowPosition(50, 0);

  // cria a janela com o título "castelo"
  glutCreateWindow("Castelo");

  // define a função de callback para redesenhar a janela
  glutDisplayFunc(displayCastelo);

  // define a função de callback para teclas normais
  glutKeyboardFunc(keyboardCastelo);

  // define a função de callback para teclas especiais
  glutSpecialFunc(SpecialKey);

  // habilita iluminação
  glEnable(GL_LIGHTING);

  // habilita a luz 0
  glEnable(GL_LIGHT0);

  // define o modelo de sombreamento como suave
  glShadeModel(GL_SMOOTH);

  // habilita o teste de profundidade (Z-buffer)
  glEnable(GL_DEPTH_TEST);

  // habilita a normalização dos vetores normais
  glEnable(GL_NORMALIZE);

  // habilita o uso da cor do material
  glEnable(GL_COLOR_MATERIAL);

  // cria um menu com as opções definidas na função "menu"
  glutCreateMenu(menuCastelo);

  // adiciona as entradas do menu
  glutAddMenuEntry("Rotacionar", 1);
  glutAddMenuEntry("Visao normal", 2);
  glutAddMenuEntry("Visao de passaro", 3);
  glutAddMenuEntry("Estrutura em malha", 4);
  glutAddMenuEntry("Estrutura solida", 5);
  glutAddMenuEntry("Modo caminhada", 6);
  glutAddMenuEntry("Sair", 7);

  // atribui o menu ao botão direito do mouse
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // inicializa parâmetros e variáveis
  init();

  // inicia o loop principal da aplicação
  glutMainLoop();
}
