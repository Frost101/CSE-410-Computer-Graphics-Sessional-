#include<bits/stdc++.h>
using namespace std;
#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include<windows.h>
    #include<glut.h>
#endif

bool animate = true;
void axes(){
        glColor3f(0,1,0);
        glBegin(GL_LINES);{
        glVertex3f(-100,0,0);
        glVertex3f(100,0,0);

        glVertex3f(0,-100,0);
        glVertex3f(0,100,0);

        glVertex3f(0,0,-100);
        glVertex3f(0,0,100);
    }glEnd();
}

void square(double a){
      glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
      
}


int counter = 0;
void display(){
    if(animate)counter++;
    printf("we are in display func  %d\n",counter);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(10, -30 , 10, 0, 0, 0, 0,1,0);

    axes();
    for(int i=0; i<4; i++){
        glPushMatrix();
        glRotatef(i*90,1,0,0);
        glTranslatef(0,0,2);
        glColor3f(1,0,0);
        square(2);
        glPopMatrix();

        glPushMatrix();
        glRotatef(i*90,0,1,0);
        glTranslatef(0,0,2);
        glColor3f(0,0,1);
        square(2);
        glPopMatrix();
    }


    //glFlush();
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

void keyHandler(unsigned char key, int x, int y){
    switch (key)
    {
    case 'v':
        printf("V pressed**************************\n");
        animate = !animate;
        break;
    
    default:
        printf("Ulta palta key");
        break;
    }
}


int main(int argc, char** argv){
    printf("Hello world\n");
    glutInit(&argc,argv);
    glutInitWindowPosition(1000,200);
    glutInitWindowSize(480,480);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL Demo");

    init();

    glutDisplayFunc(display);

    //glutIdleFunc(idle);
    //glutTimerFunc(1000,timer,0);
    glutKeyboardFunc(keyHandler);

    glutMainLoop();
    return 0;
}