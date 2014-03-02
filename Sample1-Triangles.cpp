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

double vertex1[] = {-0.5,-0.33,0.0};
double vertex2[] = {0.5,-0.33,0.0};
double vertex3[] = {0.0,0.66,0.0};
double height = 600.0;
double width = 600.0;
int mode = 0;
int count = 1;

void refresh(void)
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glBegin(GL_TRIANGLES);
      glVertex3dv(vertex1);
      glVertex3dv(vertex2);
      glVertex3dv(vertex3);
   glEnd();
   glutSwapBuffers();
}

void resize(int x, int y) 
{
    std::cout << "x = " <<  x << " y = "<< y << std::endl;
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
}

void mouseFunc(int boto, int state, int xclic, int yclic) 
{
  if (boto == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mode == 2) {  
    if (count == 1) {
      vertex1[0] = (xclic*2.0/width)-1;
      vertex1[1] = ((height-yclic)*2.0/height)-1;
    }else if (count == 2) {
      vertex2[0] = (xclic*2.0/width)-1;
      vertex2[1] = ((height-yclic)*2.0/height)-1;
    }else if(count == 3) {
      vertex3[0] = (xclic*2.0/width)-1;
      vertex3[1] = ((height-yclic)*2.0/height)-1;
    }
    count++;
  }
  glutPostRedisplay();
}

void motionFunc(int x, int y)
{
  if (mode == 1) {
    double auxX = x/width;
    double auxY = y/height;
    glClearColor(0.5,auxY,auxX,0);
    glutPostRedisplay();
  }
}

void keyboardFunc(unsigned char c, int x, int y) 
{
  mode = 0;
  count = 1;
  if (c == 27) exit(0);
  else if (c == 'f') {
    mode = 1;
  }
  else if (c == 't') {
    mode = 2;
  }
  else if (c == 'h') {
      std::cout << "Ajuda: " << std::endl;
      std::cout << "Tecla \"ESC\": Tanca l'aplicació" << std::endl;
      std::cout << "Tecla \"h\": Mostra l'ajuda" << std::endl;
      std::cout << "Tecla \"t\": Mode de creació de triangle, pitja en 3 punts de la pantalla" << std::endl;
      std::cout << "Tecla \"f\": Mode canvi de color de fons, pitjan a la pantalla i arrosegan el ratoli" << std::endl;
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