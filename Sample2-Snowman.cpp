#if defined (__APPLE__) 
  #include <OpenGL/OpenGL.h>
  #include <GLUT/GLUT.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif
#include <iostream>

//compila comando: g++ -o nameOutput -lglut -lGL arxiu.cpp
// EN Mac : g++ -o sesio1 -framework OpenGL -framework GLUT -Wno-deprecated  sesion1.cpp

double height = 600.0;
double width = 600.0;
int antX, antY;
int mode = 0;
double escalat = 1;
int girX = 0;
int girY = 0;

void pintarEixosMon() 
{
   glBegin(GL_LINES);
      glColor3f(1,0,0);
      glVertex3d(0,0,0);
      glVertex3d(0,1,0);
      glColor3f(0,1,0);
      glVertex3d(0,0,0);
      glVertex3d(1,0,0);
      glColor3f(0,0,1);
      glVertex3d(0,0,0);
      glVertex3d(0,0,1);
   glEnd();
}

void pintaNino()
{
  glPushMatrix();
  glColor3d(1,1,1);
  glutWireSphere(0.4,30,30);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0,0.6,0);
  glutWireSphere(0.2,30,30);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.1,0.6,0);
  glRotatef(90,0,1,0);
  glutWireCone(0.1,0.2,10,10);
  glPopMatrix();
}

void refresh(void)
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   pintarEixosMon();
   glColor3f(1,0,1);
   glScaled(escalat,escalat,escalat);
   glRotated(girX,0,1,0);
   glRotated(girY,1,0,0);
   pintaNino();
   glPopMatrix();
   glutSwapBuffers();
}

void resize(int x, int y) 
{
    //std::cout << "x = " <<  x << " y = "<< y << std::endl;
    height = y;
    width = x;
    int aux = x - y;
    if (aux > 0) glViewport(aux/2,0,y,y);
    else glViewport(0,(-1*aux)/2,x,x);
}

void initGL(void) 
{
  //std::cout << "x = " <<  x << " y = "<< y << std::endl;
  glClearColor(0.5,0.5,1,0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1,1,-1,1,-1,1);
  glMatrixMode(GL_MODELVIEW);
}

void mouseFunc(int boto, int state, int xclic, int yclic) 
{
  if (boto == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    antX = xclic;
    antY = yclic;
  }
}

void motionFunc(int x, int y)
{
    //double auxX = x/width;
    //double auxY = y/height;
  if (mode == 1) {
    girX = girX - (antX - x);
    girY = girY - (antY - y);
    antX = x;
    antY = y;
    glutPostRedisplay();
  }
  else if( mode == 2) {
    //std::cout << "escalat = " << escalat << " antY = " << antY << " y = " << y << std::endl;
    escalat = escalat - 0.05*(y - antY);
    if (escalat < 0) escalat = 0;
    antY = y;
    glutPostRedisplay();
  }
}

void keyboardFunc(unsigned char c, int x, int y) 
{
    mode = 0;
    if (c == 27) exit(0);
    else if (c == 'n') {
      mode = 0;
    }
    else if (c == 'r') {
      mode = 1;
    }
    else if (c == 's') {
      mode = 2;
    }
    else if (c == 'h') {
        std::cout << "Ajuda: " << std::endl;
        std::cout << "Tecla \"ESC\": Tanca l'aplicació" << std::endl;
        std::cout << "Tecla \"h\": Mostra l'ajuda" << std::endl;
        std::cout << "Tecla \"n\": Cap interaccio" << std::endl;
        std::cout << "Tecla \"r\": Rotacio del ninot" << std::endl;
        std::cout << "Tecla \"s\": Escalat del ninot" << std::endl;
    }
}


int main(int argc, const char * argv[]) 
{
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques 1 OpenGL");

    initGL();
    glutReshapeFunc(resize);
    glutMouseFunc(mouseFunc);
    glutDisplayFunc(refresh);
    glutMotionFunc(motionFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMainLoop();
    return 0;
}
