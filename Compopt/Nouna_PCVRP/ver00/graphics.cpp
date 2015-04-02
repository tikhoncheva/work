/*
 * graphics.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: kitty
 */

#include "graphics.h"
#include <GL/glut.h>

void drawDot(GLint x, GLint y)
{ // draw dot at integer point (x, y)
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

// Main loop - draw graph
void main_loop_function()
{
	double x;
	double y;

	int intX;
	int intY;

	// Clear color (screen)
	// And depth (used internally to block obstructed objects)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Load identity matrix
	glLoadIdentity();

	for (unsigned int v = 0; v < 10; ++v)
	{
		//x = Village[v].coord.first;
		//y = Village[v].coord.second;

		intX = 10+v;//std::round(x);
		intY = 10+v;//std::round(y);

		drawDot(intX, intY);

	}

	// Swap buffers (color buffers, makes previous render visible)
	glutSwapBuffers();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
		glVertex2i(100,50);
		glVertex2i(100,130);
		glVertex2i(150,130);
	glEnd();
	glFlush();

	//glutLeaveMainLoop();

}
// Initialze OpenGL perspective matrix
void GL_Setup(int width, int height)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);	//black
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);

}
// Initialize GLUT and start main loop
int tets_opengl(int argc, char** argv)
{
	glutInit(&argc, argv);								//initialize GLUT
	glutInitWindowSize(window_width, window_height);	// set window size
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);// color buffer, double buffering
	glutCreateWindow("Map of the region");

	//glutIdleFunc(main_loop_function);
	glutDisplayFunc(myDisplay);
	GL_Setup(window_width, window_height);

	//glutMainLoop();										// pass control to GLUT

	return 0;
}
