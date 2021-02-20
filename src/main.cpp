// #include "GUI.hpp"


// int		main(int argc, char **argv)
// {
// 	std::atomic<bool> isRunning;
// 	GUI gui;

// 	isRunning = true;
// 	if (gui.initGL() == false)
// 		isRunning = false;
// 	while (isRunning)
// 	{
// 		gui.events(isRunning);
// 		gui.update();
// 		gui.render();
// 	}

//     return 0;
// }



#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
 
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <iomanip>
using std::fixed;
using std::setprecision;
using std::setw;
#include <string>
#include <stdio.h>
 
class RenderState {
 public:  
   float mouseX,mouseY,cameraX,cameraY;
   bool mouseLeftDown, mouseRightDown;
   RenderState () {    
     this->mouseX = 0;
     this->mouseY = 0;
     this->mouseLeftDown = false;
     this->mouseRightDown = false;
     this->cameraX = 0.0f;
     this->cameraY = 0.0f;  
   }
 
};
 
RenderState rs;
 
void init(void)
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
 
  RenderState* rsp = new RenderState();
  rs = *rsp;
}
 
void exit(){
  delete &rs;
}
 
void drawCoordinates(){
 
    // draw some lines
    glColor3f(1.0,0.0,0.0); // red x
    glBegin(GL_LINES);
    // x aix
 
    glVertex3f(-4.0, 0.0f, 0.0f);
    glVertex3f(4.0, 0.0f, 0.0f);
 
    glVertex3f(4.0, 0.0f, 0.0f);
    glVertex3f(3.0, 1.0f, 0.0f);
 
    glVertex3f(4.0, 0.0f, 0.0f);
    glVertex3f(3.0, -1.0f, 0.0f);
    glEnd();
 
    // y 
    glColor3f(0.0,1.0,0.0); // green y
    glBegin(GL_LINES);
    glVertex3f(0.0, -4.0f, 0.0f);
    glVertex3f(0.0, 4.0f, 0.0f);
 
    glVertex3f(0.0, 4.0f, 0.0f);
    glVertex3f(1.0, 3.0f, 0.0f);
 
    glVertex3f(0.0, 4.0f, 0.0f);
    glVertex3f(-1.0, 3.0f, 0.0f);
    glEnd();
 
    // z 
    glColor3f(0.0,0.0,1.0); // blue z
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0f ,-4.0f );
    glVertex3f(0.0, 0.0f ,4.0f );
 
 
    glVertex3f(0.0, 0.0f ,4.0f );
    glVertex3f(0.0, 1.0f ,3.0f );
 
    glVertex3f(0.0, 0.0f ,4.0f );
    glVertex3f(0.0, -1.0f ,3.0f );
    glEnd();
 
}


void drawPoint(float x0, float y0, float z0)
{
        glPointSize(4);
        glBegin(GL_POINTS);
        glColor3f(255, 255.0, 0.0) ;
        glVertex3f(x0,
                   y0,
                   z0);
        glEnd();
}

void drawPlain(float x0, float y0, float z0, float x1, float y1, float z1, float rows, float PS)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);

	glColor3f(0.5, 0.5, 0.5);
	float flatLVL = 25 / rows / PS + y0;
	glVertex3f(x1, flatLVL, z0);
	glVertex3f(x0, flatLVL, z0);
	glVertex3f(x0, flatLVL, z1);
	glVertex3f(x1, flatLVL, z1);

	glEnd();
}


void showText(float x, float y, float z)
{
	glColor3f(1, 1, 1);
	glRasterPos3f(0.0, 4.5f, 0.0f);


	std::string str = "X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z);
	for (size_t i = 0; i < str.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
	}
}

void drawCube(float x0, float y0, float z0, float x2, float y2, float z2)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glVertex3f(x2, y2, z0);
	glVertex3f(x0, y2, z0);
	glVertex3f(x0, y2, z2);
	glVertex3f(x2, y2, z2);

	glVertex3f(x2, y0, z2);
	glVertex3f(x0, y0, z2);
	glVertex3f(x0, y0, z0);
	glVertex3f(x2, y0, z0);

	glVertex3f(x2, y2, z2);
	glVertex3f(x0, y2, z2);
	glVertex3f(x0, y0, z2);
	glVertex3f(x2, y0, z2);

	// back
	glVertex3f(x2, y0, z0);
	glVertex3f(x0, y0, z0);
	glVertex3f(x0, y2, z0);
	glVertex3f(x2, y2, z0);

	//left
	glVertex3f(x0, y2, z2);
	glVertex3f(x0, y2, z0);
	glVertex3f(x0, y0, z0);
	glVertex3f(x0, y0, z2);

	// right
	glVertex3f(x2, y2, z0);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y0, z2);
	glVertex3f(x2, y0, z0);

	glEnd();

}



void display(void)
{
	float Ip[] = { -120, -91.747, 126.556 };
	float vr[] = {1, 0, 0};
	float vc[] = { 0,  -0.002029,  -0.999998 };

	int rows = 320;
	int columns = 320;
	float PS = 0.75;
	float pixel_coords[] = { 150, 180 };
	float SBS = 1;
	int n = 200;

	float p0_norm[] = { Ip[0] / PS / rows, 
						Ip[1] / PS / rows, 
						Ip[2] / PS / rows };

	float p1_norm[] = { p0_norm[0] + rows / columns,
						p0_norm[1] + ((n - 1) * SBS) / rows / PS,
						p0_norm[2] + rows / columns
						};
	
	float pixel_pos[] = {pixel_coords[0] / rows + p0_norm[0],
						 25.0f / rows / PS + p0_norm[1],
						 pixel_coords[1] / rows + p0_norm[2]};

	float P3d[3];
	for (int i = 0; i < 3; i++)
		P3d[i] = Ip[i] + pixel_coords[0] * vr[i] * PS + pixel_coords[1] * vc[i] * PS;

	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();
	glTranslatef(0, 0, -10.0f);
	glRotatef(rs.cameraX, 1, 0, 0);
	glRotatef(rs.cameraY, 0, 1, 0);


	drawCoordinates();
	drawCube(p0_norm[0], p0_norm[1], p0_norm[2], p1_norm[0], p1_norm[1], p1_norm[2]);
	drawPlain(p0_norm[0], p0_norm[1], p0_norm[2], p1_norm[0], p1_norm[1], p1_norm[2], rows, PS);
	drawPoint(pixel_pos[0], pixel_pos[1], pixel_pos[2]);
	
	showText(P3d[0], P3d[1], P3d[2]);

	glutSwapBuffers();


}
 
void mouseCallback(int button, int state, int x, int y)
{
    rs.mouseX = x;
    rs.mouseY = y;
 
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            rs.mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            rs.mouseLeftDown = false;
    }
}
 
void mouseMotionCallback(int x, int y)
{
    if(rs.mouseLeftDown)
    {
        rs.cameraY += (x - rs.mouseX);
        rs.cameraX += (y - rs.mouseY);
        rs.mouseX = x;
        rs.mouseY = y;
    }
}
 
 
void idleCallback()
{
    glutPostRedisplay();
}


 
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutIdleFunc(idleCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotionCallback);

	glViewport (0, 0, (GLsizei) 500, (GLsizei) 500);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode (GL_MODELVIEW);

	glutMainLoop();
	exit();
	return 0;
}