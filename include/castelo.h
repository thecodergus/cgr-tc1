#ifndef FD3251F6_FBDE_4449_8537_A27DA2FC24C1
#define FD3251F6_FBDE_4449_8537_A27DA2FC24C1

#include <GL/glut.h>
#include <math.h>

// função de callback para teclas especiais (setas, teclas de função, etc.)
static void SpecialKey(int key, int x, int y);

// função para desenhar a bandeira
void flag();

// função para desenhar uma caixa
void box();

// função para desenhar um cilindro com base nas coordenadas de seus pontos finais, raio e número de subdivisões, e usando um objeto quadric
void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions, GLUquadricObj *quadric);

// função conveniente para desenhar um cilindro com base nas coordenadas de seus pontos finais, raio e número de subdivisões, sem usar um
// objeto quadric
void renderCylinder_convenient(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions);

// função para desenhar o castelo com base no raio fornecido
void fort(double rang);

// função de callback para animação
void inc();

// função para inicializar parâmetros e variáveis
void init();

// função de callback para redesenhar a janela
void Display();

// função de callback para teclas normais
void keyboard_castelo(unsigned char key, int x, int y);

// função de callback para o menu
void menu(int id);

// função para abrir a janela do castelo e iniciar a aplicação
void abrirCastelo();

#endif /* FD3251F6_FBDE_4449_8537_A27DA2FC24C1 */
