#ifndef __CANVAS_2D__H__
#define __CANVAS_2D__H__

//funcoes para desenho de ponto e linha
void cv_point(float x, float y);

void cv_line(float x1, float y1, float x2, float y2);

//desenha um retangulo alinhado nos eixos x e y
void cv_rect    (float x1, float y1, float x2, float y2);
void cv_rectFill(float x1, float y1, float x2, float y2);

//desenha um poligono CONVEXO. Para um retangulo, deve-se passar 4 vertices
void cv_polygon    (float x[], float y[], int n_elems);
void cv_polygonFill(float x[], float y[], int n_elems);

//centro e raio do circulo
void cv_circle    (float x, float y, float radius, int div);
void cv_circleFill(float x, float y, float radius, int div);

//especifica a cor de desenho e de limpeza de tela
void cv_color(float r, float g, float b, float a);
void cv_clear(float r, float g, float b, float a);

//desenha texto na coordenada (x,y)
void cv_text(float x, float y, const char *t);

//coordenada de offset para desenho de objetos.
void cv_translate(float x, float y);

//funcao de inicializacao da Canvas2D. Recebe a largura, altura, e um titulo para a janela
void cv_init(int w, int h, const char *title);

//funcao para executar a Canvas2D
void cv_run();

#endif
