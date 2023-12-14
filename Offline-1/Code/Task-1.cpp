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

bool animate = true;

//* Ball-Sphere
int moveAngle = 0;
int moveAngleRate = 10;
double speed = 0;
double initialAngleDegree = 45;
double radius = 1;
double radius2 = 1;
double sectors = 20;
double stacks = 20;
bool isForward = true;

point dir;

point ballPosition;
point ballDirection;
point rightDirection;
point ballUp;





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


void drawSphereHelper(double radius,int sectors,int stacks)
{
	struct point points[500][500];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=sectors;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)sectors)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)sectors)*2*pi);
			points[i][j].z=h;
		}
	}


	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		
        for(j=0;j<sectors;j++)
		{
			// glBegin(GL_QUADS);{
			//     //upper hemisphere
			// 	glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
			// 	glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
			// 	glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
			// 	glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
            //     //lower hemisphere
            //     glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
			// 	glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
			// 	glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
			// 	glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			// }glEnd();

            if(j%2) glColor3f(1,1,0);
            else glColor3f(0,1,0);
           
            glBegin(GL_TRIANGLES);{
                glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z);
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
            }glEnd();
            
            glBegin(GL_TRIANGLES);{
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
            }glEnd();
            

		}
	}
}



void drawBox(double a, int whichColor){
    glBegin(GL_QUADS);{
        if(whichColor == 0){
            glColor3f(0,0,0);
        }else{
            glColor3f(1,1,1);
        }
		glVertex3f( 0, 0, 0);
		glVertex3f( a, 0, 0);
		glVertex3f( a, a, 0);
		glVertex3f( 0, a, 0);
    }glEnd();
}


void drawChekerBoard(double a){
    for(int i=-100; i<100; i++){
        for(int j=-100; j<100; j++){
            glPushMatrix();
            glTranslatef(i*a,j*a,0);
            drawBox(a,(i+j)%2);
            glPopMatrix();
        }
    }
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


void drawSphere(double radius,int sectors,int stacks)
{
    glPushMatrix();
    {
        glTranslatef(0,0,radius);
        drawSphereHelper(radius,sectors,stacks);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0,0,radius);
        glRotatef(180,0,1,0);
        drawSphereHelper(radius,sectors,stacks);
    }
    glPopMatrix();

}


void drawBallDirection(){
    glPushMatrix();
    {
        glTranslatef(ballPosition.x, ballPosition.y, radius);
        glColor3f(0,0,1);
        glLineWidth(3);
        glBegin(GL_LINES);{
            glVertex3f(0,0,0);
            glVertex3f(2 * ballDirection.x, 2  * ballDirection.y,2 * ballDirection.z);
        }glEnd();
    }
    glPopMatrix();
}

void drawBallRightDirection(){
    glPushMatrix();
    {
        glTranslatef(ballPosition.x, ballPosition.y, radius);
        glColor3f(1,0,0);
        glLineWidth(3);
        glBegin(GL_LINES);{
            glVertex3f(0,0,0);
            glVertex3f(2 * rightDirection.x, 2  * rightDirection.y, 0);
        }glEnd();
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
    

    // axes();
    drawChekerBoard(checkerBoxSize);

    glPushMatrix();
    {   

        double temp1 = ballDirection.x + rightDirection.x;
        double temp2 = ballDirection.y + rightDirection.y;
        double temp3 = rightDirection.z;
        
        double tempAngle;
        if(isForward){
            tempAngle = -moveAngle;   
            ballPosition.x += ballDirection.x * speed;
            ballPosition.y += ballDirection.y * speed;
            ballPosition.z += ballDirection.z * speed;
        }else{
            tempAngle = moveAngle;
            ballPosition.x -= ballDirection.x * speed;
            ballPosition.y -= ballDirection.y * speed;
            ballPosition.z -= ballDirection.z * speed;
        }

        // glTranslatef(ballPosition.x, ballPosition.y, ballPosition.z);
        glRotatef(tempAngle, temp1, temp2, temp3+0.5);
        drawSphere( radius, sectors, stacks);
    }
    glPopMatrix();

    drawBallDirection();
    drawBallRightDirection();
    speed = 0;

    glutSwapBuffers();
}

void init(){
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

    dir.x = 0;
    dir.y = 0;
    dir.z = 0;


    ballPosition.x = 0;
    ballPosition.y = 0;
    ballPosition.z = 0;

    double angleInRadian = (initialAngleDegree * pi) / 180.0;
    ballDirection.x = cos(angleInRadian);
    ballDirection.y = sin(angleInRadian);
    ballDirection.z = 0;

    rightDirection.x = ballDirection.x;
    rightDirection.y = -ballDirection.y;
    rightDirection.z = radius;

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
    case 'i':
        isForward = true;
        speed = 0.5;
        moveAngle = (moveAngle + moveAngleRate);
        break;
    case 'k':
        isForward = false;
        speed = 0.5;
        moveAngle = (moveAngle + moveAngleRate);
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