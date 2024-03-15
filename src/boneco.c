#include "boneco.h"

GLfloat pos[] = {0, 0, 0, 1},                                                                 // light position
    emission[] = {0, 0, 0, 1}, emission_default[] = {0, 0, 0, 1}, amb[] = {.4, .4, .4, 1.0};  // ambient intensity
                                                                                              // property for front and back
GLfloat dr = 0, moonHorizontal = 0, moonVertical = 0, snowmanMove = 0;
bool rotateRight = false, rotateLeft = false, movement = true, goDown = false, moon = true, snowmanMovement = true, sLeft = true,
     sRight = false;

// method to make ground have snow
void snow(void) {
  // need to disable lighting and depth test
  // so creation is not affected
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glBegin(GL_QUADS);
  glColor3f(.9, .9, .9);
  glVertex2f(-2.3, -.88);
  glVertex2f(2.3, -.88);
  glVertex2f(2.3, -2.3);
  glVertex2f(-2.3, -2.3);
  glEnd();
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
}

// method to create a house
void house(void) {
  glRotated(-20, 0, 1, 0);
  // roof
  glPushMatrix();
  glColor3f(.388, .2, .0039);
  glScaled(.5, .5, .5);
  glRotated(45, 0, 1, 0);
  glutSolidOctahedron();
  glPopMatrix();
  // house
  glColor3f(.871, .612, .416);
  glTranslated(0, -.38, 0);
  glutSolidCube(.73);
  // windows
  glColor3f(0, 0, 0);
  glTranslated(-.2, .13, .32);
  glutSolidCube(.12);
  glTranslated(.4, 0, 0);
  glutSolidCube(.12);
  // door
  glTranslated(-.2, -.355, .046);
  glScaled(.5, 1.1, 0);
  glutSolidCube(.23);
}

// method to create a tree
void tree(void) {
  // trunk
  glPushMatrix();
  glColor3f(.388, .2, .0039);
  GLUquadric* qobj = gluNewQuadric();  // cylinder trunk
  glRotated(90, 1, 0, 0);
  gluCylinder(qobj, .05, .05, .4, 30, 30);
  glPopMatrix();
  // tree leaves
  glColor3f(0, .415, .0156);
  glTranslated(0, -.23, 0);
  glRotated(-90, 1, 0, 0);
  glutSolidCone(.3, .3, 40, 40);
  glTranslated(0, 0, .1);
  glutSolidCone(.25, .3, 40, 40);
  glTranslated(0, 0, .1);
  glutSolidCone(.2, .3, 40, 40);
}

// method to create moon/ sun
// the moon/sun is the light source
void moonOrSun(void) {
  glTranslated(.05, 0, 0);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glMaterialfv(GL_FRONT, GL_EMISSION, emission);
  glutSolidSphere(.4, 40, 40);
  glMaterialfv(GL_FRONT, GL_EMISSION, emission_default);
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

// method that will create an entire snowman
void snowman(void) {
  // hat
  glTranslated(-.05, 0, 0);
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslated(0, .7, 0);
  glutSolidSphere(.04, 40, 40);
  glColor3f(1, 0, 0);
  glTranslated(0, -.30, 0);
  glRotated(-90, 1, 0, 0);
  glutSolidCone(.1, .3, 40, 40);
  glColor3f(1, 1, 1);
  glTranslated(0, 0, -.015);
  glutSolidTorus(.05, .07, 10, 10);
  glPopMatrix();
  // body
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslated(0, .2, 0);
  glutSolidSphere(.19, 40, 40);
  glTranslated(0.0, -.45, 0);
  glutSolidSphere(.3, 40, 40);
  glTranslated(0.0, -.6, 0);
  glutSolidSphere(.4, 40, 40);
  glPopMatrix();
  // eyes
  glPushMatrix();  // feature
  glPushMatrix();  // face
  glTranslated(-.07, .3, .15);
  glColor3f(0, 0, 0);
  glutSolidSphere(.02, 40, 40);
  glTranslated(.13, 0, 0);
  glutSolidSphere(.02, 40, 40);
  glColor3f(.8, .3, 0);
  // nose
  glTranslated(-.065, -.06, .028);
  glutSolidCone(.03, .2, 40, 40);
  // mouth
  glColor3f(0, 0, 0);
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
  glPopMatrix();  // pop face objects
  // body buttons
  glColor3f(0, 0, 0);
  glTranslated(.03, -.09, .26);
  glutSolidSphere(.02, 40, 40);
  glTranslated(0, -.09, .03);
  glutSolidSphere(.02, 40, 40);
  glTranslated(0, -.09, .01);
  glutSolidSphere(.02, 40, 40);
  glPopMatrix();  // pop features objects
  // arms
  glPushMatrix();
  glColor3f(.388, .2, .0039);
  GLUquadric* qobj = gluNewQuadric();  // create cylinder object
  glRotated(45, 0, 0, 1);
  glRotated(90, 0, 1, 0);
  glTranslated(-.04, -.3, 0);
  gluCylinder(qobj, .02, .02, .3, 30, 30);
  glRotated(-90, 1, 0, 0);
  glTranslated(0, .3, .3);
  gluCylinder(qobj, .02, .02, .3, 30, 30);
  glPopMatrix();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  snow();  // snow on the ground

  // big snow man creation
  glPushMatrix();
  glScaled(1.5, 1.5, 1.5);
  glTranslated(-.8, -.1, -.4);
  glRotated(-15, 0, 1, 0);
  // key command animations
  if (rotateRight) {
    glRotated(dr, 0, 1, 0);
  } else if (rotateLeft) {
    glRotated(dr, 0, 1, 0);
  }
  snowman();
  glPopMatrix();

  // little snowmen creation
  glPushMatrix();
  glTranslated(-1.85, -.65, 0);
  glScaled(.25, .25, .25);
  glRotated(45, 0, 1, 0);
  // idle animation
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

  // tree creation
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

  // house creation
  glPushMatrix();
  glTranslated(.45, -.3, -1.5);
  glScaled(1.9, 1.5, 1.4);
  house();
  glPopMatrix();

  // moon creation
  glPushMatrix();
  // idle animation for moon
  if (moon == true) {
    glColor3f(.696, .696, .670);
  } else {
    glColor3f(1, 1, 0);
  }
  glTranslated(-2.7, 1.5, -.5);
  if (movement) {
    glTranslated(moonHorizontal, moonVertical, 0);
  }
  moonOrSun();
  glPopMatrix();

  glutSwapBuffers();
  glFlush();
}

void idle(void) {
  // rotation for big snowman
  if (rotateRight) {
    dr = dr + 2;
  }
  if (rotateLeft) {
    dr = dr - 2;
  }
  if ((rotateRight == false) && (rotateLeft == false)) dr = 0;

  // movement of moon/sun
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

  // movement for smaller snowmen
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
      // rotate big snowman counter-clockwise
      rotateRight = true;
      rotateLeft = false;
      break;
    case 'l':
      // rotate big snowman clockwise
      rotateLeft = true;
      rotateRight = false;
      break;
    case 'm':
      // return big snowman to default
      rotateLeft = false;
      rotateRight = false;
      break;
    case 'n':
      // night time
      moon = true;
      glClearColor(0.03, 0.02, 0.7, 0.0);
      break;
    case 'd':
      // day time
      moon = false;
      glClearColor(.165, .553, .812, 0);
      break;
  }
  glutPostRedisplay();
}

void commandInfo(void) {
  printf("Welcome to Snowman World! \n\n");
  printf("To switch to day time, press: d \n");
  printf("To switch back to night time, press: n \n");
  printf("To make the big snowman rotate clockwise, press: l \n");
  printf("To make the big snowman rotate counter-clockwise, press: k \n");
  printf("To make the big snowman return to his default animation, press: m \n");
}