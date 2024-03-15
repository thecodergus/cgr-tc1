#include <GL/glut.h>
#include <stdio.h>

#include "boneco.h"


int main(int argc, char** argv) {
  glutInit(&argc, argv);  // inicializa a biblioteca GLUT
  abrirBonecoNeve();      // chama a função para abrir a janela do boneco de neve
  return 0;               // retorna 0 indicando que o programa foi finalizado com sucesso
}
