#ifndef FD3251F6_FBDE_4449_8537_A27DA2FC24C1
#define FD3251F6_FBDE_4449_8537_A27DA2FC24C1

#include <GL/glut.h>
#include <math.h>

static void SpecialKey(int key, int x, int y);
void flag();
void box();
void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions, GLUquadricObj *quadric);
void renderCylinder_convenient(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions);
void fort(double rang);
void inc();
void init();
void Display();
void keyboard_castelo(unsigned char key, int x, int y);
void menu(int id);
void abrirCastelo();

#endif /* FD3251F6_FBDE_4449_8537_A27DA2FC24C1 */
