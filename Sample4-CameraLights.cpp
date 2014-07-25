using namespace std;
#if defined (__APPLE__) 
  #include <OpenGL/OpenGL.h>
  #include <GLUT/GLUT.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
  #include <GL/glu.h>
#endif
#include <iostream>
#include "model.h"
#include <cmath>

//compila comando: g++ -o nameOutput -lglut -lGL arxiu.cpp
// EN Mac : g++ -o sesio1 -framework OpenGL -framework GLUT -Wno-deprecated  sesion1.cpp

/*Observacions: El patricio.obj, utilitzat en l’aplicacio del bloc 3, conte, 
a mes de normal per cara, normal per vertex (tambe les contenen els models f-16.obj, cow.obj i porsche.obj)*/


/*
glutIdleFunc(); es la que se usará cuando no ocurra nada, cuando nada sea pulsado, 
en esta caso es redibujar las escena.
*/
double height = 600.0;
double width = 600.0;
int antX, antY,minX, minY, minZ, maxX, maxY, maxZ;
int mode = 0;
double escalat = 1;
float girX = 0;
float girY = 0;
float girZ = 0;
Model model;
vector<double> esfera(3);
double diametreEsfera = 0;

double escenaXmin = -5;
double escenaXmax = 5;
double escenaYmin = 0;
double escenaYmax = 2;
double escenaZmin = -5;
double escenaZmax = 5;

double near = 0;
double far = 0;
double fovy = 0;

double zoom = 1;
int zoomActive = 0;
int esferaSi = 0;

double OBSx = 0;
double OBSy = 0.7;
double OBSz = 0;
double VRPx = 0;
double VRPy = 0.7;
double VRPz = 1;

double speed = 0.05;
float angleGir = 0.0f;
float angleGirX = 0.0f;
float angleGirZ = 0.05f;

int girMult = 0;

int llums = 0;
int normalCara = 0;
int smooth = 1;
int backface = 0;
int ambientllum = 0;

int llum1 = 0;
int llum0 = 0;
int llum2 = 0;
int llum0Position = 0;


void capsa_visio(Model& model) {
	minX = maxX = model.vertices()[0];
	minY = maxY = model.vertices()[1];
	minZ = maxZ = model.vertices()[2];
	for (int i = 3; i < model.vertices().size(); i = i+3) {
		if (model.vertices()[i] > maxX) maxX = model.vertices()[i];
		else if (model.vertices()[i] < minX) minX = model.vertices()[i];
		if (model.vertices()[i+1] > maxY) maxY = model.vertices()[i+1];
		else if (model.vertices()[i+1] < minY) minY = model.vertices()[i+1];
		if (model.vertices()[i+2] > maxZ) maxZ = model.vertices()[i+2];
		else if (model.vertices()[i+2] < minZ) minZ = model.vertices()[i+2];    
	}
}

void pintaEsferaContenidora() {
	glPushMatrix();
	glColor3d(0.7,0.2,0.0);
	glTranslated(esfera[0],esfera[1],esfera[2]);
	glutWireSphere(diametreEsfera/2,50,50);
	glPopMatrix();
}

void esfera_conteidora(){
  
  double x, y, z;
  
  x = (escenaXmax+escenaXmin)/2.0;
  y = (escenaYmax+escenaYmin)/2.0;
  z = (escenaZmax+escenaZmin)/2.0;
  
  esfera[0] = x;
  esfera[1] = y;
  esfera[2] = z;
  
  x = escenaXmax - escenaXmin;
  y = escenaYmax - escenaYmin;
  z = escenaZmax - escenaZmin;
  
  diametreEsfera = sqrt(x*x+y*y+z*z); 
  
}


void pinta_model(double initialX, double initialY ,double initialZ, double grausX, double grausY, double grausZ, double dimensio) {
	glPushMatrix();
	glTranslated(initialX,initialY,initialZ);
	float dimension = dimensio/(double(maxY)-double(minY));
	glScaled(dimension, dimension, dimension);
	glRotated(grausX,1,0,0);
	glRotated(grausY,0,1,0);
	glRotated(grausZ,0,0,1);
	glTranslated(-((maxX + minX)/2.0), -minY, -((maxZ + minZ)/2.0));
	if (smooth == 0) glShadeModel(GL_FLAT);
  else glShadeModel(GL_SMOOTH);
	for (int i = 0; i < model.faces().size(); ++i) {
		const Face  &f = model.faces()[i];
		glColor4fv(Materials[f.mat].diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Materials[f.mat].diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Materials[f.mat].ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Materials[f.mat].specular);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Materials[f.mat].shininess);
		glBegin(GL_TRIANGLES);
		if(normalCara == 0 || f.n.size() == 0) glNormal3dv(model.faces()[i].normalC);
		else glNormal3dv(&model.normals()[f.n[0]]);
		glVertex3dv(&model.vertices()[f.v[0]]);
		if(normalCara == 1 && f.n.size() != 0) glNormal3dv(&model.normals()[f.n[1]]);
		glVertex3dv(&model.vertices()[f.v[1]]);
		if(normalCara == 1 && f.n.size() != 0) glNormal3dv(&model.normals()[f.n[2]]);
		glVertex3dv(&model.vertices()[f.v[2]]);
		glEnd();
	}
	glPopMatrix();
}

void pintarEixosMon() 
{
   glPushMatrix();
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
   glPopMatrix();
}

void pintaSnowman(double x, double y, double z)
{
  glPushMatrix();
  /*
  Reflexion ambient: representa el color que va a tener la zona oscura que tendra el material (GL_AMBIENT = (R,G,B,1.0)).
  Reflexion diffuse: es el colo del objeto (GL_DIFFUSE = (R,G,B,1.0)).
  Reflexion specular: indica el color que va a tener la zona de maxima luz del objeto (GL_SPECULAR = (R,G,B,1.0)).
  Shininess: Define la cantidad de puntos luminosos y su concentración. Digamos que variando este parámetro 
  podemos conseguir un objeto más o menos cercano al metal por ejemplo (GL_SHININESS = [0,128]).
  */
  float Diffuse[4] = {0.980, 0.90, 0.90,0}; 
  float ambient[4] = {0.300, 0.30, 0.30,0};
  float specular[4] = {1.000, 1.00, 1.00,0};
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diffuse);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,120);
  glColor3d(1,1,1);
  glTranslated(x,y+0.4,z);
  glutSolidSphere(0.4,30,30);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(x,y+0.99,z);
  glutSolidSphere(0.2,30,30);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(x+0.1,y+1,z);
  glRotatef(90,0,1,0);
  glColor3d(1,0.6,0);
  float ConeMaterial[4] = {1.000, 0.549, 0.000,0};
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,ConeMaterial);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ConeMaterial);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,ConeMaterial);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,120);
  glutSolidCone(0.1,0.2,10,10);
  glPopMatrix();
}

void setLlumGroga()
{
    if (llum0 == 1) {
      GLfloat light_ambient[] = { 0.25, 0.25, 0.1, 0.0 };
      GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 0.0 };
      GLfloat light_specular[] = { 1.0, 1.0, 0.0, 0.0 };
      if (llum0Position == 0) { 
        GLfloat light_position[] = { 5.0, 0.2, 5.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      } else if (llum0Position == 1) {
        GLfloat light_position[] = { -5.0, 0.2, 5.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      } else if (llum0Position == 2) {
        GLfloat light_position[] = { -5.0, 0.2, -5.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      } else if (llum0Position == 3) {
        GLfloat light_position[] = { 5.0, 0.2, -5.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      } else {
        GLfloat light_position[] = { 0.0, 0.2, 0.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      }
      glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
      glEnable(GL_LIGHT0);
    }
}

void pintaTerra()
{
    glPushMatrix();

    int contI = 0;
    int contJ = 0;    
    
    for (int i = -5; i < 5; i+=2) {
      for (int j = -5; j < 5; j+=2) {
          if ((contI%2 == 0 && contJ%2 == 1) || (contI%2 == 1 && contJ%2 == 1)) {
            float Diffuse[4] = {0.1, 0.1, 0.1,1.0};
            float ambient[4] = {0.05, 0.05, 0.05,1.0};
            float specular[4] = {0, 0, 0,1.0};
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diffuse);
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
            glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,120);
          }else {
            float Diffuse[4] = {0.9, 0.9, 0.9,1.0};
            float ambient[4] = {1, 1, 1,1.0};
            float specular[4] = {1, 1, 1,1.0};
            glMaterialfv(GL_FRONT,GL_DIFFUSE,Diffuse);
            glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
            glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
            glMaterialf(GL_FRONT,GL_SHININESS,120);
          }
          glBegin(GL_QUADS);
            if ((contI%2 == 0 && contJ%2 == 1) || (contI%2 == 1 && contJ%2 == 1)) glColor3d(0,0,0); 
            else glColor3d(1,1,1);
            glNormal3f(0,1,0);
            glVertex3d(i,0,j);
            glNormal3f(0,1,0);
            glVertex3d(i,0,j+2);
            glNormal3f(0,1,0);
            glVertex3d(i+2,0,j+2);
            glNormal3f(0,1,0);
            glVertex3d(i+2,0,j);
         glEnd();
         ++contJ;
      }
      ++contI;
    }
  

  glPopMatrix();
}

void pintaParet(double initialX, double initialY, double initialZ, double costatX, double costatY, double costatZ)
{
    glPushMatrix();
    float Diffuse[4] = {0.678, 1.000, 0.184,0};
    float ambient[4] = {0.000, 0.392, 0.000,0};
    float specular[4] = {0, 1, 0,0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,120);
    glTranslated(initialX,initialY+costatY/2,initialZ);
    glScaled(costatX,costatY,costatZ);
    glNormal3f(0,1,0);
    glutSolidCube(1);
    glPopMatrix();
}

void setEuler() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.,0.,-diametreEsfera);
    if (girMult == 1) {
      
      glRotated(-girZ,0.,0.,1.);
      glRotated(girY,1.,0.,0.);
      GLdouble m[16];
      glGetDoublev(GL_MODELVIEW_MATRIX, m);
      glLoadIdentity();
      glRotated(-girX, 0.0, 1.0, 0.0);
      glMultMatrixd(m);

    } else {
      glRotated(-girZ,0.,0.,1.);
      glRotated(girY,1.,0.,0.);
      glRotated(-girX,0.,1.,0.);
    }
    glTranslated(-esfera[0],-esfera[1],-esfera[2]);
}

void refresh(void)
{ 
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   if (backface == 1) glEnable(GL_CULL_FACE);
   else glDisable(GL_CULL_FACE);
   if (ambientllum == 1) {
      GLfloat lmodel_ambient [] = { 1, 0, 0, 0 };
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   }
   else {
      GLfloat lmodel_ambient [] = { 0.2, 0.2, 0.2, 1.0 };
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   }
   if (mode != 6 && mode != 7) setEuler();
   glColor3f(1,0,1);
   if (esferaSi == 1) pintaEsferaContenidora();

   setLlumGroga();
   pintaTerra();
   pintaSnowman(2.5, 0, 2.5);
   pintaSnowman(-2.5, 0, 2.5);
   pintaSnowman(-2.5, 0, -2.5);

   if (mode != 6 && mode != 7) pinta_model(0,0,0,0,0,0,1); //lloc x , y ,z , rotat x , y , z ,dimensio 
   pinta_model(2.5,0,-2.5,0,90,0,1.5); //lloc x , y ,z , rotat x , y , z , dimensio 
   if(mode != 4) {
     pintaParet(2.5,0,-1.5,4,1.5,0.2);
     pintaParet(-4.9,0,0,0.2,1.5,10);
   }
   //pintarEixosMon();
   glutSwapBuffers();
}

void moure_camera(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(OBSx,OBSy,OBSz,VRPx,VRPy,VRPz,0,1,0);
  if (llum2 == 1) {
      GLfloat light_ambient[] = { 1.0, 1.0, 0.0, 0.0 };
      GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 0.0 };
      GLfloat light_specular[] = { 1.0, 1.0, 0.0, 0.0 };
      GLfloat light_position[] = { OBSx,OBSy,OBSz, 1};
      glLightfv(GL_LIGHT2, GL_POSITION, light_position);
      glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
      glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
      glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
      glEnable(GL_LIGHT2);
  }
  glutPostRedisplay();
}

void cambiar_camara() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (mode != 5) {
      if (width > height) {
	double aux = width/height;
	glOrtho(-diametreEsfera/2*aux*zoom, diametreEsfera/2*aux*zoom, -diametreEsfera/2*zoom, diametreEsfera/2*zoom, -1, diametreEsfera+5);
      }
      else {
	double aux = height/width;
	glOrtho(-diametreEsfera/2*zoom, diametreEsfera/2*zoom, -diametreEsfera/2*aux*zoom, diametreEsfera/2*aux*zoom, -1, diametreEsfera+5);
      }  
  } else {
      double anglecam = asin((diametreEsfera/2)/diametreEsfera)*180./M_PI; //equivalent a R/distancia
      double ratio = width/height;
      if (ratio >= 1) fovy= anglecam * 2.0;
      else {
	      double DEG2GRAD = M_PI/180.;
	      double RAD2DEG = 180./M_PI;
	      near = 5;
	      fovy = 2.0 * atan(tan(anglecam*DEG2GRAD)/ratio)*RAD2DEG;
      }
      gluPerspective(fovy*zoom, ratio, 1,diametreEsfera*2+1);
  }
  glMatrixMode(GL_MODELVIEW);
}

void cambiar_camara_patricio() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double ratio = width/height;
  gluPerspective(100, ratio, 0.1,diametreEsfera);
  glMatrixMode(GL_MODELVIEW);
}


void resize(int x, int y) 
{
    height = y*1.0;
    width = x*1.0;
    glViewport(0,0,width,height);
    if (mode != 6 && mode != 7) cambiar_camara();
    glutPostRedisplay();
}


void initGL(void) 
{
  //std::cout << "x = " <<  x << " y = "<< y << std::endl;
  glClearColor(0.5,0.5,1,0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  esfera_conteidora();
  glOrtho(-diametreEsfera/2, diametreEsfera/2,-diametreEsfera/2,diametreEsfera/2, 1, diametreEsfera);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  

  GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };

  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glEnable(GL_LIGHT1);
}

void mouseFunc(int boto, int state, int xclic, int yclic) 
{
  if (boto == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mode != 7) {
    antX = xclic;
    antY = yclic;
    zoomActive = 0;
  } else if (boto == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && mode != 7 ) {
    zoomActive = 1;
    antY = yclic;
    antX = xclic;
  }else if (boto == GLUT_RIGHT_BUTTON && state == GLUT_UP && mode != 7) {
    zoomActive = 0;
    antY = yclic;
    antX = xclic;
  } else if (boto == GLUT_RIGHT_BUTTON && mode == 7 && state == GLUT_DOWN) {
    angleGir += 0.15;
    angleGirX = -sin(angleGir);
    angleGirZ = cos(angleGir);
    VRPz = OBSz+angleGirZ;
    VRPx = OBSx+angleGirX;
    moure_camera();
  } else if (boto == GLUT_LEFT_BUTTON && mode == 7 && state == GLUT_DOWN) {
    angleGir -= 0.15;
    angleGirX = -sin(angleGir);
    angleGirZ= cos(angleGir);
    VRPz = OBSz+angleGirZ;
    VRPx = OBSx+angleGirX;
    moure_camera();
  }
}

void motionFunc(int x, int y)
{
  if (zoomActive) {
    	zoom = zoom + 0.05*(y-antY);
    	if (zoom <= 0) zoom = 0.05;
    	if (zoom * fovy > 180) zoom =  180/fovy;
    	antX = x;
    	antY = y;
    	cambiar_camara();
    	glutPostRedisplay();
  } else {
      if (mode == 1) {
      	girX = girX - (antX - x);
      	if (girX < 0) girX = 0;
      	else if (girX > 360) girX = 360;
      	girY = girY + (antY - y);
      	if (girY < -90) girY = -90;
      	else if (girY > 90) girY = 90;
      	antX = x;
      	antY = y;
      	glutPostRedisplay();
      }   
  }
}

void move_esquerra(){
    angleGir -= 0.02;
    angleGirX = -sin(angleGir);
    angleGirZ= cos(angleGir);
    VRPz = OBSz+angleGirZ;
    VRPx = OBSx+angleGirX;
    moure_camera();
}

void move_dreta() {
    angleGir += 0.02;
    angleGirX = -sin(angleGir);
    angleGirZ = cos(angleGir);
    VRPz = OBSz+angleGirZ;
    VRPx = OBSx+angleGirX;
    moure_camera();
}

void move_endavant() {
    VRPz = VRPz + speed*angleGirZ;
    VRPx = VRPx + speed*angleGirX;
    OBSz = OBSz + speed*angleGirZ;
    OBSx = OBSx + speed*angleGirX;
    moure_camera();
}

void move_endarere() {
    VRPz = VRPz - speed*angleGirZ;
    VRPx = VRPx - speed*angleGirX;
    OBSz = OBSz - speed*angleGirZ;
    OBSx = OBSx - speed*angleGirX;
    moure_camera();
}

void move_endavant_tafaner() {
    OBSz = OBSz + speed*(cos(0));
    OBSx = OBSx + speed*(-sin(0));
    moure_camera();
}

void move_endarere_tafaner() {
    OBSz = OBSz - speed*(cos(0));
    OBSx = OBSx - speed*(-sin(0));
    moure_camera();
}

void keyboardFunc(unsigned char c, int x, int y) 
{
  switch (c) {
    
    case 27: 
      exit(0);
      break;
    
    case 'r': //Rota l'escena
      mode = 1;
      break;

    case 'l': //Situació inicial
      mode = 0;
      zoom = 1;
      girX = 0;
      girY = 0;
      girZ = 0;
      cambiar_camara();
      glutPostRedisplay();
      break;

    case 'e': //Esfera contenidora
      if (esferaSi == 1) esferaSi = 0;
      else esferaSi = 1;
      glutPostRedisplay();
      break;

    case 'i': //Encendre llums
      if (llums == 1) {
        glDisable(GL_LIGHTING);
        llums = 0;
        llum2 = 0;
        llum1 = 0;
        llum0 = 0;
      } else {
        glEnable(GL_LIGHTING);
        llum1 = 1;
        llum2 = 1;
        llum0 = 1;
        llums = 1;
      }
      glutPostRedisplay();
      break;

    case '1': //Encendre apagar la llum d'escena
      if (llum1 == 1){
        llum1 = 0;
        glDisable(GL_LIGHT1);
      } else {
        llum1 = 1;
        glEnable(GL_LIGHT1);
      }
      glutPostRedisplay();
      break;

    case '0': //Encendre apagar la llum de les cantonades
      if (llum0 == 1){
        llum0 = 0;
        glDisable(GL_LIGHT0);
      } else {
        llum0 = 1;
        glEnable(GL_LIGHT0);
      }
      glutPostRedisplay();
      break;

    case '2': //Encendre apagar la llum del patricio cuan camina
      if (llum2 == 1){
        llum2 = 0;
        glDisable(GL_LIGHT2);
      } else {
        llum2 = 1;
        glEnable(GL_LIGHT2);
      }
      if (mode == 6) moure_camera();
      break;

    case 'm': //Moure la llum0 a difarents cantonades
      llum0Position++;
      if (llum0Position > 4) llum0Position = 0;
      glutPostRedisplay();
      break;

    case 'c': //Moure la camara al patricio "caminar"
      if (mode == 6) { //Tornem a l'estat inicial
          mode = 0;
          zoom = 1;
          girX = 0;
          girY = 0;
          girZ = 0;
          cambiar_camara();
          glutPostRedisplay();
        }
        else mode = 6;
        cambiar_camara_patricio();
        moure_camera();
        break;

    case 'a':
      if (mode == 6) move_esquerra();
      break;

    case 'd':
      if (mode == 6) move_dreta();
      break;

    case 'w':
      if (mode == 6) move_endavant();
      if (mode == 7) move_endavant_tafaner();
      break;

    case 's':
      if (mode == 6) move_endarere(); 
      if (mode == 7) move_endarere_tafaner();
      break;

    case 'z':
      if (mode == 6 && speed <= 5) speed+=0.1; 
      break;

    case 'x':
      if(mode == 6 && speed > 0.01) speed -= 0.01;
      break;

    case 'n': //Normals per vertex o per cara
      if (normalCara == 0) normalCara = 1;
      else normalCara = 0;
      glutPostRedisplay();
      break;

    case 'o': //Pintar amb smooth or flat shading
      if (smooth == 1) smooth = 0;
      else smooth = 1;
      glutPostRedisplay();
      break;

    case 'b': //backface culling
      if (backface == 0) backface = 1;
      else backface = 0;
      glutPostRedisplay();
      break;

    case 'k': //lum ambient apagar encendre
      if (ambientllum == 0) ambientllum = 1;
      else ambientllum = 0;
      glutPostRedisplay();
      break;

    case 'g': //gir amb multiplicacio
      if (girMult == 0) girMult = 1;
      else girMult = 0;
      glutPostRedisplay();
      break;

    case 'v': //Pinta paretos o no
      if (mode == 4) mode = 0;
      else mode = 4;
      glutPostRedisplay();
      break;

    case 'p': //cambiar camera
      if (mode == 5) mode = 0;
      else mode = 5;
      cambiar_camara();
      glutPostRedisplay();
      break;

    case 'h':
      std::cout << "Ajuda: " << std::endl;
      std::cout << "Tecla \"ESC\": Tanca l'aplicació" << std::endl;
      std::cout << "Tecla \"h\": Mostra l'ajuda" << std::endl;
      std::cout << "Tecla \"r\": Rotar l'escena" << std::endl;
      std::cout << "Tecla \"e\": Pinta o borrar esfera contenedora" << std::endl;
      std::cout << "Tecla \"l\": estat inicial" << std::endl;
      std::cout << "Tecla \"p\": Cambia de camera" << std::endl;
      std::cout << "Tecla \"c\": Caminar mode walk" << std::endl;
      std::cout << "Tecles \"a,d,w,s\": caminar amb el mode walk" << std::endl;
      std::cout << "Tecla \"z,x\": augmentar i disminuir velocitat del mode walk" << std::endl;
      std::cout << "Tecla \"n\": Pinta amb les normals per cara o vertex" << std::endl;
      std::cout << "Tecla \"i\": Encendre/apagar llums" << std::endl;
      std::cout << "Tecla \"0\": Encendre/apagar llum0" << std::endl;
      std::cout << "Tecla \"1\": Encendre/apagar llum1" << std::endl;
      std::cout << "Tecla \"2\": Encendre/apagar llum2" << std::endl;
      std::cout << "Tecla \"v\": Pinta les parets o no" << std::endl;
      break;
  }
}


int main(int argc, const char * argv[]) 
{
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques 4 OpenGL");

    initGL();
    glutReshapeFunc(resize);
    glutMouseFunc(mouseFunc);
    glutDisplayFunc(refresh);
    glutMotionFunc(motionFunc);
    glutKeyboardFunc(keyboardFunc);
    model.load(argv[1]);
    capsa_visio(model);
    glutMainLoop();
    return 0;
}
