#ifndef B6665F8F_AF03_4FEC_BEF6_16DEDAAE8F4B
#define B6665F8F_AF03_4FEC_BEF6_16DEDAAE8F4B

// Inclusão de bibliotecas dependendo do sistema operacional
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Inclusão de bibliotecas padrão
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Declaração de funções utilizadas no código
void snow(void);
void house(void);
void tree(void);
void moonOrSun(void);
void snowman(void);
void displayBoneco(void);
void idle(void);
void keyboard(unsigned char key, int x, int y);
void commandInfo(void);
void abrirBonecoNeve();

#endif /* B6665F8F_AF03_4FEC_BEF6_16DEDAAE8F4B */
