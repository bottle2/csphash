/**
*   Programa para ilustrar os elementos mais basicos do OpenGL e Glut.
*   - Apresenta os principais recursos do sistema de Janelas GLUT
*
*   Autor: Cesar Tadeu Pozzer
*   UFSM - 2020
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
* Referencias GLUT: http://www.opengl.org/documentation/specs/glut/
*                   http://www.opengl.org/documentation/specs/glut/spec3/node1.html
**/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#define PI_2 6.28318530717
#define PI   3.14159265359

#ifndef Y_CANVAS_CRESCE_PARA_CIMA
#define Y_CANVAS_CRESCE_PARA_CIMA 1
#endif

#include "gl_canvas2d.h"

static int screen_height; //guarda referencia para as variaveis de altura e largura da main()

void ConvertMouseCoord(int button, int state, int wheel, int direction, int x, int y);

//funcoes de CALLBACK da biblioteca Glut
void keyboard(int key);
void keyboardUp(int key);
void specialUp(int key, int ignored1, int ignored2);
void mouse(int bt, int st, int wheel, int direction, int x, int y);
void mouseWheelCB(int wheel, int direction, int x, int y);
void resize(int width, int height);
void update(void);

void cv_point(float x, float y)
{
   glBegin(GL_POINTS);
      glVertex2f(x, y);
   glEnd();
}

void cv_line(float x1, float y1, float x2, float y2)
{
   glBegin(GL_LINES);
      glVertex2f(x1, y1);
      glVertex2f(x2, y2);
   glEnd();
}

void cv_rect(float x1, float y1, float x2, float y2)
{
   glBegin(GL_LINE_LOOP);
      glVertex2f(x1, y1);
      glVertex2f(x1, y2);
      glVertex2f(x2, y2);
      glVertex2f(x2, y1);
   glEnd();
}

void cv_rectFill(float x1, float y1, float x2, float y2)
{
   glBegin(GL_QUADS);
      glVertex2f(x1, y1);
      glVertex2f(x1, y2);
      glVertex2f(x2, y2);
      glVertex2f(x2, y1);
   glEnd();
}

void cv_polygon(float x[], float y[], int elems)
{
   glBegin(GL_LINE_LOOP);
      for(int cont=0; cont<elems; cont++)
      {
         glVertex2f(x[cont], y[cont]);
      }
   glEnd();
}

void cv_polygonFill(float x[], float y[], int elems)
{
   glBegin(GL_POLYGON);
      for(int cont=0; cont<elems; cont++)
      {
         glVertex2f(x[cont], y[cont]);
      }
   glEnd();

}

//existem outras fontes de texto que podem ser usadas
//  GLUT_BITMAP_9_BY_15
//  GLUT_BITMAP_TIMES_ROMAN_10
//  etc. Para mais detalhes, acesse https://www.opengl.org/resources/libraries/glut/spec3/node76.html
//Vejam tambem a funcao glutStrokeWidth(GLUTstrokeFont font, int character)
//Para textos de qualidade, ver:
//  https://www.freetype.org/
//  http://ftgl.sourceforge.net/docs/html/ftgl-tutorial.html
void cv_text(float x, float y, const char *t)
{
    int tam = (int)strlen(t);
    for(int c=0; c < tam; c++)
    {
      glRasterPos2i(x + c*10, y);
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, t[c]);
    }
}

void cv_clear(float r, float g, float b, float a)
{
   glClearColor(r, g, b, a);
}

void cv_circle(float x, float y, float radius, int div )
{
   float ang = 0;
   float inc = PI_2/div;
   glBegin(GL_LINE_LOOP);
      for(int lado = 1; lado <= div; lado++) //GL_LINE_LOOP desenha um poligono fechado. Liga automaticamente o primeiro e ultimio vertices.
      {
         glVertex2f(cos(ang) * radius + x, sin(ang) * radius + y);
         ang += inc;
      }
   glEnd();
}

void cv_circleFill(float x, float y, float radius, int div )
{
   float ang = 0;
   float inc = PI_2/div;
   glBegin(GL_POLYGON);
      for(int lado = 1; lado <= div; lado++) //GL_POLYGON desenha um poligono CONVEXO preenchido.
      {
         glVertex2f(cos(ang) * radius + x, sin(ang) * radius + y);
         ang+=inc;
      }
   glEnd();
}

//coordenada de offset para desenho de objetos.
//nao armazena translacoes cumulativas.

void cv_translate(float x, float y)
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(x, y, 0);
}

void cv_color(float r, float g, float b, float a)
{
   glColor4f(r, g, b, a);
}

void special(int key, int ignored1, int ignored2)
{
    (void)ignored1;
    (void)ignored2;
    keyboard(key+100);
}

void specialUp(int key, int ignored1, int ignored2)
{
    (void)ignored1;
    (void)ignored2;
    keyboardUp(key+100);
}

void keyb(unsigned char key, int ignored1, int ignored2)
{
    (void)ignored1;
    (void)ignored2;
    keyboard(key);
}

void keybUp(unsigned char key, int ignored1, int ignored2)
{
    (void)ignored1;
    (void)ignored2;
    keyboardUp(key);
}

void mouseClick(int button, int state, int x, int y)
{
   ConvertMouseCoord(button, state, -2, -2, x, y);
}

void mouseWheelCB(int wheel, int direction, int x, int y)
{
   ConvertMouseCoord(-2, -2, wheel, direction, x, y);
}

void motion(int x, int y)
{
   ConvertMouseCoord(-2, -2, -2, -2, x, y);
}

void ConvertMouseCoord(int button, int state, int wheel, int direction, int x, int y)
{
#if Y_CANVAS_CRESCE_PARA_CIMA == TRUE
   y = screen_height - y; //deve-se inverter a coordenada y do mouse se o y da canvas crescer para cima. O y do mouse sempre cresce para baixo.
#else
   //nao faz nada.
#endif
   mouse(button, state, wheel, direction, x, y);
}

//funcao chamada sempre que a tela for redimensionada.
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   //cria uma projecao ortografica com z entre (-1, 1).
#if Y_CANVAS_CRESCE_PARA_CIMA == TRUE
   //parametros: left, right, bottom, top
   gluOrtho2D (0.0, w, 0.0, h); //o eixo y cresce para cima.
#else
   //parametros: left, right, bottom, top
   gluOrtho2D (0.0, w, h, 0.0); //o eixo y cresce para baixo
#endif

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();

   resize(w, h);

   screen_height = h;
}

//definicao de valores para limpar buffers
void inicializa(void)
{
   glClearColor(1,1,1,1);
   glPolygonMode(GL_FRONT, GL_FILL);
}

void display (void)
{
   glClear(GL_COLOR_BUFFER_BIT );

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   update();

   glFlush();
   glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////////////
//  inicializa o OpenGL
////////////////////////////////////////////////////////////////////////////////////////
void cv_init(int w, int h, const char *title)
{
   int argc = 0;
   glutInit(&argc, NULL);

   //habilita MSAA
   glutSetOption(GLUT_MULTISAMPLE, 8);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
   //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

   glutInitWindowSize (w, h);
   glutInitWindowPosition (50, 50);
   glutCreateWindow (title);

   inicializa();

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyb);
   glutKeyboardUpFunc(keybUp);
   glutSpecialUpFunc(specialUp);
   glutSpecialFunc(special);

   glutIdleFunc(display);
   glutMouseFunc(mouseClick);
   glutPassiveMotionFunc(motion);
   glutMotionFunc(motion);
   glutMouseWheelFunc(mouseWheelCB);

#if 0
   printf("GL Version: %s\n", glGetString(GL_VERSION));
#endif
}

void cv_run(void)
{
   glutMainLoop();
}

