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

//*
double steps = 16.0;

//* Triangle Scale
double currentLengthTriangle = 5;
double maximumLengthTriangle = 5;
double triangleSteps = currentLengthTriangle / steps;

//* Sphere scale
double antiClockAngle = 0;
double currentRadiusSphere = 0;
double maximumRadiusSphere = maximumLengthTriangle / sqrt(3.0);
double sphereSteps = maximumRadiusSphere / steps;
double sphereMoveX = currentLengthTriangle;



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
            glTranslated((maximumLengthTriangle - currentLengthTriangle) / 3.0,
                         (maximumLengthTriangle - currentLengthTriangle) / 3.0,
                          (maximumLengthTriangle - currentLengthTriangle) / 3.0);
            glScaled(currentLengthTriangle, currentLengthTriangle, currentLengthTriangle);
            drawTriangle();
        }
        glPopMatrix();
    }
}


//* From : https://www.songho.ca/opengl/gl_sphere.html
// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the given longitude/latitude angles
void drawSphereface(int subdivision, double radius)
{
    const double DEG2RAD = acos(-1) / 180.0f;

    std::vector<double> vertices;
    double n1[3];        // normal of longitudinal plane rotating along Y-axis
    double n2[3];        // normal of latitudinal plane rotating along Z-axis
    double v[3];         // direction vector intersecting 2 planes, n1 x n2
    double a1;           // longitudinal angle along Y-axis
    double a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    //* To save the points
    struct point pointsOnSphere[pointsPerRow][pointsPerRow];

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            pointsOnSphere[i][j].x = v[0] * radius * 1.0;
            pointsOnSphere[i][j].y = v[1] * radius * 1.0;
            pointsOnSphere[i][j].z = v[2] * radius * 1.0;
        }
    }

    //* Draw sphere Face
    int n = pointsPerRow - 1;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            glBegin(GL_QUADS);{
                glVertex3d(pointsOnSphere[i][j].x, pointsOnSphere[i][j].y, pointsOnSphere[i][j].z);
                glVertex3d(pointsOnSphere[i][j+1].x, pointsOnSphere[i][j+1].y, pointsOnSphere[i][j+1].z);
                glVertex3d(pointsOnSphere[i+1][j+1].x, pointsOnSphere[i+1][j+1].y, pointsOnSphere[i+1][j+1].z);
                glVertex3d(pointsOnSphere[i+1][j].x, pointsOnSphere[i+1][j].y, pointsOnSphere[i+1][j].z);
            }glEnd();
        }
    }
}



void convertToSphere(){
    currentRadiusSphere += sphereSteps;
    currentLengthTriangle -= triangleSteps;

    sphereMoveX -= triangleSteps;

    if(currentRadiusSphere >= maximumRadiusSphere || currentLengthTriangle <= 0){
        currentLengthTriangle = 0;
        currentRadiusSphere = maximumRadiusSphere;
        sphereMoveX = 0;
    }
}

void convertToOctahedron(){
    currentRadiusSphere -= sphereSteps;
    currentLengthTriangle += triangleSteps;

    sphereMoveX += triangleSteps;

    if(currentRadiusSphere <= 0 || currentLengthTriangle >= maximumLengthTriangle){
        currentLengthTriangle = maximumLengthTriangle;
        currentRadiusSphere = 0;
        sphereMoveX = maximumLengthTriangle;
    }
}



void drawSphereFromFaces(int subdivision, double radius){
    for(int i=0; i<4; i++){
        glPushMatrix();
        {
            if(i%2)glColor3f(0, 1, 0);
            else glColor3f(0, 0, 1);
            glRotated(i*90, 0, 0, 1);
            glTranslated(currentLengthTriangle, 0, 0);
            drawSphereface(subdivision, radius);
        }
        glPopMatrix();
    }

    glPushMatrix();
    {
        glColor3f(1, 0, 0);
        glRotated(90, 0, 1, 0);
        glTranslated(currentLengthTriangle, 0, 0);
        drawSphereface(subdivision, radius);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3f(1, 0, 0);
        glRotated(-90, 0, 1, 0);
        glTranslated(currentLengthTriangle, 0, 0);
        drawSphereface(subdivision, radius);
    }
    glPopMatrix();
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


void drawWithRotation(int subdivision, double radius){
    glPushMatrix();
    {
        glRotated(antiClockAngle, 0, 0, 1);
        drawOctahedron();
        drawSphereFromFaces(subdivision, radius);
    }
    glPopMatrix();
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
    
    // drawSphereface(6, 4);
    drawWithRotation(8, currentRadiusSphere);

    

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
    case 'a':
        antiClockAngle -= 10;
        break;
    case 'd':
        antiClockAngle += 10;
        break;
    case ',':
        convertToSphere();
        break;
    case '.':
        convertToOctahedron();
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