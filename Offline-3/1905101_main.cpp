//* Ray Tracing Assignment

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include<windows.h>
    #include<glut.h>
#endif

#define PLUS_INF 1000000000

#include<bits/stdc++.h>
#include "1905123_classes.h"
#include "bitmap_image.hpp"
using namespace std;


//* Function Prototypes
void display();
void normalKeyHandler(unsigned char key, int x, int y);
void specialKeyHandler(int key, int x, int y);
void init();
void axes();
void loadData();


//* Camera
Camera camera;

//* Save the objects and lights
vector <Object*> objects;
vector <PointLight*> pointLights;
vector <SpotLight*> spotLights;



int level_of_recursion = 0;
int image_height_width = 0;
int object_count = 0;
int light_point_count = 0;
int spot_light_count = 0;
bitmap_image image;
double viewAngle = 80.0;
int window_width = 680;
int window_height = 680;





void Capture(){

    //* Initialize bitamp image and set background color to black;
    // image.set_all_channels(0, 0, 0);
    for(int i=0; i<image_height_width; i++){
        for(int j=0; j<image_height_width; j++){
            image.set_pixel(i, j, 0, 0, 0);
        }
    }

    //* planeDistance = (window_height/2) / tan(viewAngle/2)
    double viewAngleRad = (viewAngle * M_PI) / 180.0;
    double planeDistance = (window_height/2.0) / tan(viewAngleRad/2.0);

    //* topleft = eye + l * planeDistance - r * (window_width/2) + u * (window_height/2)
    Point topleft;
    topleft.x = camera.eye.x + (camera.lookAt.x * planeDistance) - (camera.right.x * (window_width/2.0)) + (camera.up.x * (window_height/2.0));
    topleft.y = camera.eye.y + (camera.lookAt.y * planeDistance) - (camera.right.y * (window_width/2.0)) + (camera.up.y * (window_height/2.0));
    topleft.z = camera.eye.z + (camera.lookAt.z * planeDistance) - (camera.right.z * (window_width/2.0)) + (camera.up.z * (window_height/2.0));


    //* du = window_width / image_width
    //* dv = window_height / image_height
    double du = (window_width * 1.0) / (image_height_width * 1.0);
    double dv = (window_height * 1.0) / (image_height_width * 1.0);

    //* Choose middle of the grid cell
    //* topleft = topleft + r * (0.5 * du) - u * (0.5 * dv)
    topleft.x = topleft.x + (camera.right.x * (0.5 * du)) - (camera.up.x * (0.5 * dv));
    topleft.y = topleft.y + (camera.right.y * (0.5 * du)) - (camera.up.y * (0.5 * dv));
    topleft.z = topleft.z + (camera.right.z * (0.5 * du)) - (camera.up.z * (0.5 * dv));

    //* Track the nearest object Index and the nearest t 
    

    for(int i=0; i<image_height_width; i++){
        for(int j=0; j<image_height_width; j++){

            int nearestObjectIndex = -1;
            double nearestT = PLUS_INF;
            
            //* calculate curPixel using topleft, r, u, i, j, du, dv
            Point curPixel;
            curPixel.x = topleft.x + (camera.right.x * i * du) - (camera.up.x * j * dv);
            curPixel.y = topleft.y + (camera.right.y * i * du) - (camera.up.y * j * dv);
            curPixel.z = topleft.z + (camera.right.z * i * du) - (camera.up.z * j * dv);

            //* Cast a ray from eye to (curPixel - eye) dirction
            Point rayDirection;
            rayDirection.x = curPixel.x - camera.eye.x;
            rayDirection.y = curPixel.y - camera.eye.y;
            rayDirection.z = curPixel.z - camera.eye.z;
            rayDirection.normalize();
            Ray ray(camera.eye, rayDirection); 

            //* Initialize a dummy color
            Color color;

            //* for each object, o in objects
            //*     t = o->intersect(ray,color,0)
            //*     update t so that it stores min +ve value
            //*     save the nearest object, On

            
            for(int k=0; k<objects.size(); k++){
                double t = objects[k]->intersect(&ray, &color, 0);
                if(t > 0 && (t < nearestT || nearestObjectIndex == -1)){
                    nearestT = t;
                    nearestObjectIndex = k;
                }
            }

            //* tmin = On->intersect(ray,color,1)
            if(nearestObjectIndex != -1){
                Color color;
                double tmin = objects[nearestObjectIndex]->intersect(&ray, &color, 1);

                //* Clip the color values between 0 and 1
                if(color.r < 0) color.r = 0;
                if(color.g < 0) color.g = 0;
                if(color.b < 0) color.b = 0;

                if(color.r > 1) color.r = 1;
                if(color.g > 1) color.g = 1;
                if(color.b > 1) color.b = 1;

                //* Set the pixel color
                image.set_pixel(i, j, color.r * 255, color.g * 255, color.b * 255);   
            }


        }
    }

    //* Save the image
    image.save_image("out.bmp");
    cout << "Image saved" << endl;


}


void axes(){
    glBegin(GL_LINES);{
        glColor3f(1,0,0);
        glVertex3f(-200,0,0);
        glVertex3f(200,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,-200,0);
        glVertex3f(0,200,0);


        glColor3f(0,0,1);
        glVertex3f(0,0,-200);
        glVertex3f(0,0,200);
    }glEnd();
}



void loadData(){
    //* Read datas from "scene.txt" file

    ifstream sceneFile;
    sceneFile.open("scene.txt");

    if(!sceneFile){
        cout << "File not found\n";
        return;
    }

    sceneFile >> level_of_recursion;
    sceneFile >> image_height_width;

    //* Initialize the bitmap image
    image = bitmap_image(image_height_width, image_height_width);

    sceneFile >> object_count;

    for(int i=0; i<object_count; i++){
        string object_type;
        sceneFile >> object_type;

        if(object_type == "triangle"){
            /*
            Example:

            -20.0 -20.0 0.0	- x1, y1, z1
            20.0 -20.0 0.0	- x2, y2, z2
            0.0 0.0 20.0	- x3, y3, z3
            1.0 0.0 0.0	- color
            0.4 0.2 0.1 0.3	- ambient, diffuse, specular, recursive reflection coefficient
            5		- shininess

            */

            Point p1, p2, p3;
            Color color;

            sceneFile >> p1.x >> p1.y >> p1.z;
            sceneFile >> p2.x >> p2.y >> p2.z;
            sceneFile >> p3.x >> p3.y >> p3.z;

            sceneFile >> color.r >> color.g >> color.b;

            double ambient, diffuse, specular, recursive_reflection_coefficient;
            sceneFile >> ambient >> diffuse >> specular >> recursive_reflection_coefficient;

            double shininess;
            sceneFile >> shininess;

            // Object *triangle = new Triangle(p1, p2, p3, color, ambient, diffuse, specular, recursive_reflection_coefficient, shininess);
            // objects.push_back(triangle);

            cout << "Triangle\n";
            cout << p1.x << " " << p1.y << " " << p1.z << endl;
            cout << p2.x << " " << p2.y << " " << p2.z << endl;
            cout << p3.x << " " << p3.y << " " << p3.z << endl;
            cout << color.r << " " << color.g << " " << color.b << endl;
            cout << ambient << " " << diffuse << " " << specular << " " << recursive_reflection_coefficient << endl;
            cout << shininess << endl;
            cout << endl;
        }
        else if(object_type == "sphere"){
            /*
            Example:
            40.0 0.0 10.0	- center
            10.0		- radius
            0.0 1.0 0.0	- color
            0.4 0.2 0.2 0.2	- ambient, diffuse, specular, recursive reflection coefficient
            5		- shininess
            */

            Point center;
            double radius;
            Color color;

            sceneFile >> center.x >> center.y >> center.z;
            sceneFile >> radius;
            sceneFile >> color.r >> color.g >> color.b;

            double ambient, diffuse, specular, recursive_reflection_coefficient;
            sceneFile >> ambient >> diffuse >> specular >> recursive_reflection_coefficient;

            double shininess;
            sceneFile >> shininess;

            Object *sphere = new Sphere(center, radius, color, ambient, diffuse, specular, recursive_reflection_coefficient, shininess);
            objects.push_back(sphere);
           

            cout << "Sphere\n";
            cout << center.x << " " << center.y << " " << center.z << endl;
            cout << radius << endl;
            cout << color.r << " " << color.g << " " << color.b << endl;
            cout << ambient << " " << diffuse << " " << specular << " " << recursive_reflection_coefficient << endl;
            cout << shininess << endl;
            cout << endl;
        }
        else if(object_type == "general"){
            /*
            Example:
            1 1 1 0 0 0 -20 -20 -20 200	- A B C D E F G H I J
            0 0 0 0 0 5	- cube reference point, length, width, height (0 indicates no clipping along this dimension)
            0.0 0.0 1.0	- color
            0.4 0.2 0.1 0.3	- ambient, diffuse, specular, recursive reflection coefficient
            3		- shininess
            */

            double A, B, C, D, E, F, G, H, I, J;
            sceneFile >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;

            Point cube_reference_point;
            double length, width, height;
            sceneFile >> cube_reference_point.x >> cube_reference_point.y >> cube_reference_point.z;
            sceneFile >> length >> width >> height;

            Color color;
            sceneFile >> color.r >> color.g >> color.b;

            double ambient, diffuse, specular, recursive_reflection_coefficient;
            sceneFile >> ambient >> diffuse >> specular >> recursive_reflection_coefficient;

            double shininess;
            sceneFile >> shininess;

            cout << "General\n";
            cout << A << " " << B << " " << C << " " << D << " " << E << " " << F << " " << G << " " << H << " " << I << " " << J << endl;
            cout << cube_reference_point.x << " " << cube_reference_point.y << " " << cube_reference_point.z << endl;
            cout << length << " " << width << " " << height << endl;
            cout << color.r << " " << color.g << " " << color.b << endl;
            cout << ambient << " " << diffuse << " " << specular << " " << recursive_reflection_coefficient << endl;
            cout << shininess << endl;
            cout << endl;
        }
    }

    sceneFile >> light_point_count;
    cout << "Light Point Count: " << light_point_count << endl;

    for(int i=0; i<light_point_count; i++){
        /*
        Example:
        70.0 70.0 70.0	- position of the 1st point light source
        1.0 0.0 0.0	- color of the 1st point light source
        */

        Point light_position;
        Color light_color;
        
        sceneFile >> light_position.x >> light_position.y >> light_position.z;
        sceneFile >> light_color.r >> light_color.g >> light_color.b;

        PointLight *pointLight = new PointLight(light_position, light_color);
        pointLights.push_back(pointLight);

        cout << "Light\n";
        cout << light_position.x << " " << light_position.y << " " << light_position.z << endl;
        cout << light_color.r << " " << light_color.g << " " << light_color.b << endl;
        cout << endl;
    }

    sceneFile >> spot_light_count;
    cout << "Spot Light Count: " << spot_light_count << endl;

   

    for(int i=0; i<spot_light_count; i++){
        /*
        Example:
        100 100 -200	- position of the 1st spotlight source
        0 1.0 0.0	- color of the 1st spotlight source
        0 0.0 1		- direction of the 1st spotlight source
        12		- cutoff angle (in degree) of the 1st spotlight source
        */

        Point light_position;
        Color light_color;
        Point light_direction;
        double cutoff_angle;

        sceneFile >> light_position.x >> light_position.y >> light_position.z;
        sceneFile >> light_color.r >> light_color.g >> light_color.b;
        sceneFile >> light_direction.x >> light_direction.y >> light_direction.z;
        sceneFile >> cutoff_angle;

        SpotLight *spotLight = new SpotLight(light_position, light_color, light_direction, cutoff_angle);
        spotLights.push_back(spotLight);

        cout << "Spot Light\n";
        cout << light_position.x << " " << light_position.y << " " << light_position.z << endl;
        cout << light_color.r << " " << light_color.g << " " << light_color.b << endl;
        cout << light_direction.x << " " << light_direction.y << " " << light_direction.z << endl;
        cout << cutoff_angle << endl;
        cout << endl;
    }


    Floor *floor = new Floor(1000, 20);
    floor->setCoEfficients(0.25, 0.25, 0.25, 0.25);
    floor->setShine(15);
    // objects.push_back(floor);

    cout << "Load data done" << endl;
    sceneFile.close();
    

}


void init(){
	//codes for initialization
    loadData();

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();


	gluPerspective(80,	1,	1,	1000.0);
	
}


void display(){
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //* Set Camera LookAt
    camera.setCamera();
    

    //* Draw the axis
    axes();

   
    
    //*Draw the objects
    for(int i=0; i<objects.size(); i++){
        objects[i]->draw();
    }

    //* Draw the point lights
    for(int i=0; i<pointLights.size(); i++){
        pointLights[i]->draw();
    }

    //* Draw the spot lights
    for(int i=0; i<spotLights.size(); i++){
        spotLights[i]->draw();
    }

    

   
    glutSwapBuffers();
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
        camera.tiltClockwise();
        break;
    case '6':
        camera.tiltAntiClockwise();
        break;
    case '0':
        Capture();
        break;
    default:
        printf("Ulta palta key\n");
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
        printf("You have pressed the wrong key\n");
        break;
    }

    glutPostRedisplay();
}


int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitWindowPosition(1000,200);
    glutInitWindowSize(window_width, window_height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL Demo");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(normalKeyHandler);
    glutSpecialFunc(specialKeyHandler);

    glutMainLoop();
    return 0;
}