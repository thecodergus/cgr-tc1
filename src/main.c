#include <GL/glut.h>
#include <stdio.h>

#include "boneco.h"

int main(int argc, char** argv) {
  GLfloat front_amb_diff[] = {.7, .5, .1, 1.0};  // front side property
  GLfloat back_amb_diff[] = {.4, .7, .1, 1.0};   // back side property
  GLfloat spe[] = {.2, .2, .2, 1.0};

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 700);
  glutInitWindowPosition(200, 100);
  glutCreateWindow("Snowman World");
  glClearColor(0.03, 0.02, 0.7, 0.0);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1.0, 2, 8);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff);
  glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_amb_diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(0, 0, -5);       // translate object (& light) into view volume
  glEnable(GL_COLOR_MATERIAL);  // to change the colors of 3d generated objects
  glEnable(GL_NORMALIZE);       // for scaling down objects
  commandInfo();                // display key command information
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutMainLoop();
}