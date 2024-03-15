#include <GL/glut.h>
#include <stdio.h>

#include "boneco.h"

void abrirBonecoNeve() {
  GLfloat front_amb_diff[] = {.7, .5, .1, 1.0};  // propriedade do lado frontal
  GLfloat back_amb_diff[] = {.4, .7, .1, 1.0};   // propriedade do lado traseiro
  GLfloat spe[] = {.2, .2, .2, 1.0};

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);        // define o modo de exibição
  glutInitWindowSize(800, 700);                                    // define o tamanho da janela
  glutInitWindowPosition(200, 100);                                // define a posição da janela
  glutCreateWindow("Boneco de Neve");                              // cria a janela com o título "Snowman World"
  glClearColor(0.03, 0.02, 0.7, 0.0);                              // define a cor de fundo da janela
  glEnable(GL_DEPTH_TEST);                                         // habilita o teste de profundidade
  glMatrixMode(GL_PROJECTION);                                     // define a matriz de projeção
  glLoadIdentity();                                                // carrega a matriz identidade
  gluPerspective(45, 1.0, 2, 8);                                   // define a perspectiva
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);      // define as propriedades de cor do material
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff);  // define a cor do lado frontal
  glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_amb_diff);    // define a cor do lado traseiro
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);               // define a cor especular
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30);                // define a brilhantez
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);                 // habilita o modelo de iluminação de dois lados
  glMatrixMode(GL_MODELVIEW);                                      // define a matriz de visualização
  glLoadIdentity();                                                // carrega a matriz identidade
  glTranslated(0, 0, -5);                                          // move a cena para trás
  glEnable(GL_COLOR_MATERIAL);                                     // habilita o uso de cores definidas pelo material
  glEnable(GL_NORMALIZE);                                          // habilita o normalize das normais
  commandInfo();                                                   // exibe informações sobre os comandos disponíveis
  glutKeyboardFunc(keyboard);                                      // define a função de callback para os eventos de teclado
  glutDisplayFunc(displayBoneco);                                  // define a função de callback para o redesenho da janela
  glutIdleFunc(idle);                                              // define a função de callback para o processamento em segundo plano
  glutMainLoop();                                                  // inicia o loop principal da aplicação
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);  // inicializa a biblioteca GLUT
  abrirBonecoNeve();      // chama a função para abrir a janela do boneco de neve
  return 0;               // retorna 0 indicando que o programa foi finalizado com sucesso
}
