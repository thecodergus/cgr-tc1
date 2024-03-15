#include "boneco.h"

// Posição da luz
GLfloat pos[] = {0, 0, 0, 1},

        // Emissão da luz e emissão padrão
    emission[] = {0, 0, 0, 1}, emission_default[] = {0, 0, 0, 1},

        // Intensidade ambiente para frente e atrás
    amb[] = {.4, .4, .4, 1.0};

// Variáveis para controle de movimento
GLfloat dr = 0, moonHorizontal = 0, moonVertical = 0, snowmanMove = 0;
bool rotateRight = false, rotateLeft = false, movement = true, goDown = false, moon = true, snowmanMovement = true, sLeft = true,
     sRight = false;

// Método para criar neve no chão
void snow(void) {
  // Desabilita a iluminação e o teste de profundidade para não afetar a criação
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  // Desenha um quadrado com cor branca representando a neve
  glBegin(GL_QUADS);
  glColor3f(.9, .9, .9);
  glVertex2f(-2.3, -.88);
  glVertex2f(2.3, -.88);
  glVertex2f(2.3, -2.3);
  glVertex2f(-2.3, -2.3);
  glEnd();

  // Reabilita a iluminação e o teste de profundidade
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
}
// Método para criar uma casa
void house(void) {
  glRotated(-20, 0, 1, 0);

  // Telhado
  glPushMatrix();
  glColor3f(.388, .2, .0039);  // Cor do telhado
  glScaled(.5, .5, .5);
  glRotated(45, 0, 1, 0);
  glutSolidOctahedron();  // Forma do telhado
  glPopMatrix();

  // Corpo da casa
  glColor3f(.871, .612, .416);  // Cor da casa
  glTranslated(0, -.38, 0);
  glutSolidCube(.73);  // Forma da casa

  // Janelas
  glColor3f(0, 0, 0);  // Cor das janelas
  glTranslated(-.2, .13, .32);
  glutSolidCube(.12);  // Janela esquerda
  glTranslated(.4, 0, 0);
  glutSolidCube(.12);  // Janela direita

  // Porta
  glTranslated(-.2, -.355, .046);
  glScaled(.5, 1.1, 0);
  glutSolidCube(.23);  // Porta
}

// Método para criar uma árvore
void tree(void) {
  // Tronco
  glPushMatrix();
  glColor3f(.388, .2, .0039);          // Cor do tronco
  GLUquadric* qobj = gluNewQuadric();  // Cria um objeto quadric para desenhar o cilindro do tronco
  glRotated(90, 1, 0, 0);
  gluCylinder(qobj, .05, .05, .4, 30, 30);  // Desenha o tronco
  glPopMatrix();

  // Folhas da árvore
  glColor3f(0, .415, .0156);  // Cor das folhas
  glTranslated(0, -.23, 0);
  glRotated(-90, 1, 0, 0);
  glutSolidCone(.3, .3, 40, 40);  // Desenha as folhas da árvore (cone superior)
  glTranslated(0, 0, .1);
  glutSolidCone(.25, .3, 40, 40);  // Desenha as folhas da árvore (cone médio)
  glTranslated(0, 0, .1);
  glutSolidCone(.2, .3, 40, 40);  // Desenha as folhas da árvore (cone inferior)
}

// Método para criar a lua/sol
// A lua/sol é a fonte de luz
void moonOrSun(void) {
  glTranslated(.05, 0, 0);

  // Define a posição da luz
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  // Define a emissão do material
  glMaterialfv(GL_FRONT, GL_EMISSION, emission);

  // Desenha a esfera representando a lua/sol
  glutSolidSphere(.4, 40, 40);

  // Restaura a emissão padrão do material
  glMaterialfv(GL_FRONT, GL_EMISSION, emission_default);

  // Define a intensidade ambiente da luz
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

  // Habilita a iluminação e a luz 0
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

// Método que cria um boneco de neve completo
void snowman(void) {
  // Chapéu
  glTranslated(-.05, 0, 0);
  glPushMatrix();
  glColor3f(1, 1, 1);  // Cor branca
  glTranslated(0, .7, 0);
  glutSolidSphere(.04, 40, 40);  // Parte superior do chapéu
  glColor3f(1, 0, 0);            // Cor vermelha
  glTranslated(0, -.30, 0);
  glRotated(-90, 1, 0, 0);
  glutSolidCone(.1, .3, 40, 40);  // Parte inferior do chapéu
  glColor3f(1, 1, 1);             // Cor branca
  glTranslated(0, 0, -.015);
  glutSolidTorus(.05, .07, 10, 10);  // Aba do chapéu
  glPopMatrix();

  // Corpo
  glPushMatrix();
  glColor3f(1, 1, 1);  // Cor branca
  glTranslated(0, .2, 0);
  glutSolidSphere(.19, 40, 40);  // Cabeça
  glTranslated(0.0, -.45, 0);
  glutSolidSphere(.3, 40, 40);  // Corpo do meio
  glTranslated(0.0, -.6, 0);
  glutSolidSphere(.4, 40, 40);  // Corpo de baixo
  glPopMatrix();

  // Olhos
  glPushMatrix();
  glPushMatrix();
  glTranslated(-.07, .3, .15);
  glColor3f(0, 0, 0);            // Cor preta
  glutSolidSphere(.02, 40, 40);  // Olho esquerdo
  glTranslated(.13, 0, 0);
  glutSolidSphere(.02, 40, 40);  // Olho direito

  // Nariz
  glColor3f(.8, .3, 0);  // Cor laranja
  glTranslated(-.065, -.06, .028);
  glutSolidCone(.03, .2, 40, 40);

  // Boca
  glColor3f(0, 0, 0);  // Cor preta
  glTranslated(-.06, -.08, 0);
  glutSolidSphere(.014, 40, 40);
  glTranslated(.03, -.015, 0);
  glutSolidSphere(.014, 40, 40);
  glTranslated(.03, -.001, 0);
  glutSolidSphere(.014, 40, 40);
  glTranslated(.03, .001, 0);
  glutSolidSphere(.014, 40, 40);
  glTranslated(.03, .015, 0);
  glutSolidSphere(.014, 40, 40);
  glPopMatrix();

  // Botões do corpo
  glColor3f(0, 0, 0);  // Cor preta
  glTranslated(.03, -.09, .26);
  glutSolidSphere(.02, 40, 40);
  glTranslated(0, -.09, .03);
  glutSolidSphere(.02, 40, 40);
  glTranslated(0, -.09, .01);
  glutSolidSphere(.02, 40, 40);
  glPopMatrix();

  // Braços
  glPushMatrix();
  glColor3f(.388, .2, .0039);          // Cor marrom
  GLUquadric* qobj = gluNewQuadric();  // Cria objeto quadrático para criar o cilindro
  glRotated(45, 0, 0, 1);
  glRotated(90, 0, 1, 0);
  glTranslated(-.04, -.3, 0);
  gluCylinder(qobj, .02, .02, .3, 30, 30);  // Braço esquerdo
  glRotated(-90, 1, 0, 0);
  glTranslated(0, .3, .3);
  gluCylinder(qobj, .02, .02, .3, 30, 30);  // Braço direito
  glPopMatrix();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Neve no chão
  snow();

  // Criação do boneco de neve grande
  glPushMatrix();
  glScaled(1.5, 1.5, 1.5);
  glTranslated(-.8, -.1, -.4);
  glRotated(-15, 0, 1, 0);

  // Animações com comandos de teclado
  if (rotateRight) {
    glRotated(dr, 0, 1, 0);
  } else if (rotateLeft) {
    glRotated(dr, 0, 1, 0);
  }
  snowman();
  glPopMatrix();

  // Criação dos bonecos de neve pequenos
  glPushMatrix();
  glTranslated(-1.85, -.65, 0);
  glScaled(.25, .25, .25);
  glRotated(45, 0, 1, 0);

  // Animação de movimento
  if (snowmanMovement) {
    glRotated(snowmanMove, 0, 0, 1);
  }
  snowman();
  glPopMatrix();

  glPushMatrix();
  glTranslated(1.1, -1, -.6);
  glScaled(.5, .5, .5);
  if (snowmanMovement) {
    glRotated(-snowmanMove, 0, 0, 1);
  }
  snowman();
  glPopMatrix();

  // Criação das árvores
  glPushMatrix();
  glScaled(2.5, 4, 2.5);
  glTranslated(.85, 0, -.3);
  tree();
  glPopMatrix();

  glPushMatrix();
  glScaled(3, 4, 3);
  glTranslated(.7, 0, -.9);
  tree();
  glPopMatrix();

  // Criação da casa
  glPushMatrix();
  glTranslated(.45, -.3, -1.5);
  glScaled(1.9, 1.5, 1.4);
  house();
  glPopMatrix();

  // Criação da lua
  glPushMatrix();

  // Animação de cor da lua
  if (moon == true) {
    glColor3f(.696, .696, .670);
  } else {
    glColor3f(1, 1, 0);
  }
  glTranslated(-2.7, 1.5, -.5);

  // Movimento da lua
  if (movement) {
    glTranslated(moonHorizontal, moonVertical, 0);
  }
  moonOrSun();
  glPopMatrix();

  glutSwapBuffers();
  glFlush();
}

void idle(void) {
  // rotação do boneco de neve grande
  if (rotateRight) {
    dr = dr + 2;
  }
  if (rotateLeft) {
    dr = dr - 2;
  }
  if ((rotateRight == false) && (rotateLeft == false)) dr = 0;

  // movimento da lua/sol
  moonHorizontal = moonHorizontal + .002;
  if (goDown == false) moonVertical = moonVertical + .0004;
  if (moonHorizontal > 5.5) {
    moonHorizontal = 0;
    moonVertical = 0;
    goDown = false;
  }
  if (moonVertical > .45 || goDown == true) {
    goDown = true;
    moonVertical = moonVertical - .0004;
  }

  // movimento dos bonecos de neve menores
  if (sLeft) {
    snowmanMove = snowmanMove + 1.5;
    if (snowmanMove >= 20) {
      sLeft = false;
      sRight = true;
    }
  }
  if (sRight) {
    snowmanMove = snowmanMove - 1.5;
    if (snowmanMove <= -20) {
      sLeft = true;
      sRight = false;
    }
  }
  glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'k':
      // rotaciona o boneco de neve grande no sentido anti-horário
      rotateRight = true;
      rotateLeft = false;
      break;
    case 'l':
      // rotaciona o boneco de neve grande no sentido horário
      rotateLeft = true;
      rotateRight = false;
      break;
    case 'm':
      // retorna o boneco de neve grande à sua posição padrão
      rotateLeft = false;
      rotateRight = false;
      break;
    case 'n':
      // define a cena para noite
      moon = true;
      glClearColor(0.03, 0.02, 0.7, 0.0);
      break;
    case 'd':
      // define a cena para dia
      moon = false;
      glClearColor(.165, .553, .812, 0);
      break;
  }
  glutPostRedisplay();
}

void commandInfo(void) {
  printf("Bem-vindo ao Mundo do Boneco de Neve! \n\n");
  printf("Para mudar para o dia, pressione: d \n");
  printf("Para mudar para a noite, pressione: n \n");
  printf("Para fazer o boneco de neve grande girar no sentido horário, pressione: l \n");
  printf("Para fazer o boneco de neve grande girar no sentido anti-horário, pressione: k \n");
  printf("Para fazer o boneco de neve grande retornar à sua animação padrão, pressione: m \n");
}
