#ifndef TEXTO_H
#define TEXTO_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b);
void PrintText(GLfloat x, GLfloat y, const char * text, double r, double g, double b);

#endif
