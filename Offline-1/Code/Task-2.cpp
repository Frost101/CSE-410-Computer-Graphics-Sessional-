#include<bits/stdc++.h>
#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include<windows.h>
    #include<glut.h>
#endif
#include "Camera.h"
#define pi (2*acos(0.0))
using namespace std;

struct point
{
	double x,y,z;
};

//* Camera
Camera camera;

//* Checker Board
int drawgrid = 1;
double checkerBoxSize = 2;

//* Triangle Scale
double triangleScale = 5;

bool animate = true;



void axes(){
        glBegin(GL_LINES);{
        glColor3f(1,0,0);
        glVertex3f(-100,0,0);
        glVertex3f(100,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,-100,0);
        glVertex3f(0,100,0);


        glColor3f(0,0,1);
        glVertex3f(0,0,-100);
        glVertex3f(0,0,100);
    }glEnd();
}

void drawTriangle(){
    glBegin(GL_TRIANGLES);{
        glVertex3f( 0, 1, 0);
        glVertex3f( 1, 0, 0);
        glVertex3f( 0, 0, 1);
    }glEnd();

}

void drawPyramid(){
    
    for(int i=0; i<=3; i++){
        if(i%2) glColor3f(1 , 0, 1);
        else glColor3f(0, 1, 1);

        glPushMatrix();
        {
            glRotatef(90*i, 0, 0, 1);
            glScaled( triangleScale, triangleScale, triangleScale);
            drawTriangle();
        }
        glPopMatrix();
    }
}



void drawOctahedron(){
    glPushMatrix();
    {
        drawPyramid();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(180, 1, 1, 0);
        glRotatef(90, 0, 0, 1);
        drawPyramid();
    }
    glPopMatrix();
}



void drawSquare(double a)
{
    glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( 0, 0, 0);
		glVertex3f( a, 0, 0);
		glVertex3f( a, a, 0);
		glVertex3f( 0, a, 0);
	}glEnd();
}



int counter = 0;
void display(){
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //* Set Camera LookAt
    camera.setCamera();
    

    axes();
    drawOctahedron();

    glutSwapBuffers();
}

void init(){
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

}

void idle(){
    printf("No job\n");
    glutPostRedisplay();
}

void timer(int value){
    glutPostRedisplay();
    printf("Timer function called %d\n",counter);
    glutTimerFunc(100,timer,0);
}


void normalKeyHandler(unsigned char key, int x, int y){
    switch (key)
    {
    case '1':
        camera.rotateLeft();
        break;
    case '2':
        camera.rotateRight();
        break;
    case '3':
        camera.lookUp();
        break;
    case '4':
        camera.lookDown();
        break;
    case '5':
        camera.tiltAntiClockwise();
        break;
    case '6':
        camera.tiltClockwise();
        break;
    
    default:
        printf("Ulta palta key");
        break;
    }

    glutPostRedisplay();
}

void specialKeyHandler(int key, int x, int y){
    switch (key)
    {
    case GLUT_KEY_UP:
        camera.moveForward();
        break;
    case GLUT_KEY_DOWN:
        camera.moveBackward();
        break;
    case GLUT_KEY_LEFT:
        camera.moveLeft();
        break;
    case GLUT_KEY_RIGHT:
        camera.moveRight();
        break;
    case GLUT_KEY_PAGE_UP:
        camera.moveUp();
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera.moveDown();
        break;
    default:
        printf("Ulta palta key");
        break;
    }

    glutPostRedisplay();
}


int main(int argc, char** argv){
    printf("Hello world\n");
    glutInit(&argc,argv);
    glutInitWindowPosition(1000,200);
    glutInitWindowSize(680,680);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL Demo");

    init();

    glutDisplayFunc(display);

    //glutIdleFunc(idle);
    //glutTimerFunc(1000,timer,0);
    glutKeyboardFunc(normalKeyHandler);
    glutSpecialFunc(specialKeyHandler);

    glutMainLoop();
    return 0;
}