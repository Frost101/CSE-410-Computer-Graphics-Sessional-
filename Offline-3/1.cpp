

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include<windows.h>
    #include<glut.h>
#endif
#include<bits/stdc++.h>

#define AMB 0
#define DIFF 1
#define SPEC 2
#define REFL 3


using namespace std;

//* Class prototypes
class Point;
class Color;
class Camera;
class Object;
class Sphere;
class Triangle;
class General;
class PointLight;
class SpotLight;
class Floor;
class Ray;



//*extern
extern vector <Object*> objects;
extern vector <PointLight*> pointLights;
extern vector <SpotLight*> spotLights;




class Point
{
public:
    double x;
    double y;
    double z;

    Point(){
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void normalize(){
        double length = sqrt(x*x + y*y + z*z);

        x = x/length;
        y = y/length;
        z = z/length;
    }
};


class Color
{
public:
    double r;
    double g;
    double b;

    Color(){
        r = 0.0;
        g = 0.0;
        b = 0.0;    }

    Color(double r, double g, double b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
};



class PointLight
{
    public:
        Point position;
        Color color;

        PointLight(){
            position.x = 0;
            position.y = 0;
            position.z = 0;

            color.r = 0;
            color.g = 0;
            color.b = 0;
        }
        
        PointLight(Point position, Color color){
            this->position.x = position.x;
            this->position.y = position.y;
            this->position.z = position.z;

            this->color.r = color.r;
            this->color.g = color.g;
            this->color.b = color.b;
        }

        void draw(){
            glPushMatrix();
            {
                glTranslatef(position.x, position.y, position.z);
                glColor3f(color.r, color.g, color.b);
                glPointSize(5);
                glBegin(GL_POINTS);{
                    glVertex3f(0, 0, 0);
                }glEnd();
            }
            glPopMatrix();
        }

};



class SpotLight
{
    public:
        PointLight pointLight;
        Point light_direction;
        double cutoff_angle;

        SpotLight(){
            pointLight.position.x = 0;
            pointLight.position.y = 0;
            pointLight.position.z = 0;

            pointLight.color.r = 0;
            pointLight.color.g = 0;
            pointLight.color.b = 0;

            light_direction.x = 0;
            light_direction.y = 0;
            light_direction.z = 0;

            cutoff_angle = 0;
        }

        SpotLight(Point position, Color color, Point light_direction, double cutoff_angle){
            this->pointLight.position.x = position.x;
            this->pointLight.position.y = position.y;
            this->pointLight.position.z = position.z;

            this->pointLight.color.r = color.r;
            this->pointLight.color.g = color.g;
            this->pointLight.color.b = color.b;

            this->light_direction.x = light_direction.x;
            this->light_direction.y = light_direction.y;
            this->light_direction.z = light_direction.z;

            this->cutoff_angle = cutoff_angle;
        }


        SpotLight(PointLight pointLight, Point light_direction, double cutoff_angle){
            this->pointLight.position.x = pointLight.position.x;
            this->pointLight.position.y = pointLight.position.y;
            this->pointLight.position.z = pointLight.position.z;

            this->pointLight.color.r = pointLight.color.r;
            this->pointLight.color.g = pointLight.color.g;
            this->pointLight.color.b = pointLight.color.b;

            this->light_direction.x = light_direction.x;
            this->light_direction.y = light_direction.y;
            this->light_direction.z = light_direction.z;

            this->cutoff_angle = cutoff_angle;
        }

        void draw(){
            pointLight.draw();
        }


};







class Camera
{
private:

    void rotate(Point *first, Point *second, double angleRate){
        Point temp1;
        temp1.x = first->x * cos(angleRate) + second->x * sin(angleRate);
        temp1.y = first->y * cos(angleRate) + second->y * sin(angleRate);
        temp1.z = first->z * cos(angleRate) + second->z * sin(angleRate);

        Point temp2;
        temp2.x = second->x * cos(angleRate) - first->x * sin(angleRate);
        temp2.y = second->y * cos(angleRate) - first->y * sin(angleRate);
        temp2.z = second->z * cos(angleRate) - first->z * sin(angleRate);

        first->x = temp1.x;
        first->y = temp1.y;
        first->z = temp1.z;

        second->x = temp2.x;
        second->y = temp2.y;
        second->z = temp2.z;
    }

    void normalize(Point *p){
        double length = sqrt(p->x*p->x + p->y*p->y + p->z*p->z);

        p->x = p->x/length;
        p->y = p->y/length;
        p->z = p->z/length;
    }
    
public:
    Point eye;
    Point lookAt;
    Point up;
    Point right;

    double angleRate;
    double moveRate;

    Camera(){
        //* Set suitable eye values so that all the objects are visible
        eye.x = 100;
        eye.y = 100;
        eye.z = 100;

        lookAt.x = -1/sqrt(3);
        lookAt.y = -1/sqrt(3);
        lookAt.z = -1/sqrt(3);

        up.x = 0;
        up.y = 0;
        up.z = 1;

        right.x = -1/sqrt(2);
        right.y = 1/sqrt(2);
        right.z = 0;



        angleRate = 0.05;
        moveRate = 0.1;

        //* Normalizing the lookAt vector
        this->normalize(&lookAt);

        //* Normalizing the right vector
        this->normalize(&right);

    }

    void setAngleRate(double rate){
        angleRate = rate;
    }

    void setMoveRate(double rate){
        moveRate = rate;
    }

    void setCamera(){
        // cout << "Camera: " << endl;
        // cout << "Eye: " << eye.x << " " << eye.y << " " << eye.z << endl;
        // cout << "LookAt: " << lookAt.x << " " << lookAt.y << " " << lookAt.z << endl;
        // cout << "Up: " << up.x << " " << up.y << " " << up.z << endl;
        // cout << "Right: " << right.x << " " << right.y << " " << right.z << endl;
        // cout << "--------------" << endl;
        gluLookAt(eye.x, eye.y, eye.z, 
                eye.x + lookAt.x, eye.y + lookAt.y, eye.z + lookAt.z,
                up.x, up.y, up.z);
    }

    void setLookAt(Point p){
        lookAt.x = p.x;
        lookAt.y = p.y;
        lookAt.z = p.z;

        //* Normalizing the lookAt vector
        this->normalize(&lookAt);
    }

    void setEye(Point p){
        eye.x = p.x;
        eye.y = p.y;
        eye.z = p.z;
    }

    void setRight(Point p){
        right.x = p.x;
        right.y = p.y;
        right.z = p.z;

        //* Normalizing the right vector
        this->normalize(&right);
    }

    void setUp(Point p){
        up.x = p.x;
        up.y = p.y;
        up.z = p.z;

        //* Normalizing the up vector
        this->normalize(&up);
    }

    void moveForward(){
        eye.x += (moveRate * (lookAt.x));
        eye.y += (moveRate * (lookAt.y));
        eye.z += (moveRate * (lookAt.z));
    }

    void moveBackward(){
        eye.x -= (moveRate * (lookAt.x));
        eye.y -= (moveRate * (lookAt.y));
        eye.z -= (moveRate * (lookAt.z));
    }

    void moveRight(){
        eye.x += (moveRate * (right.x));
        eye.y += (moveRate * (right.y));
        eye.z += (moveRate * (right.z));
    }

    void moveLeft(){
        eye.x -= (moveRate * (right.x));
        eye.y -= (moveRate * (right.y));
        eye.z -= (moveRate * (right.z));

    }

    void moveUp(){
        eye.x += (moveRate * (up.x));
        eye.y += (moveRate * (up.y));
        eye.z += (moveRate * (up.z));
    }


    void upWithNoRefChange(){
        Point temp;

        Point normalizedEye;

        normalizedEye.x = eye.x;
        normalizedEye.y = eye.y;
        normalizedEye.z = eye.z;

        this->normalize(&normalizedEye);

        temp.x = normalizedEye.x + lookAt.x;
        temp.y = normalizedEye.y + lookAt.y;
        temp.z = normalizedEye.z + lookAt.z;

        eye.x += (moveRate * (up.x));
        eye.y += (moveRate * (up.y));
        eye.z += (moveRate * (up.z));


        //* Normalize the updated eye
        normalizedEye.x = eye.x;
        normalizedEye.y = eye.y;
        normalizedEye.z = eye.z;

        this->normalize(&normalizedEye);


        lookAt.x = temp.x - normalizedEye.x;
        lookAt.y = temp.y - normalizedEye.y;
        lookAt.z = temp.z - normalizedEye.z;
    }

    void downWithNoRefChange(){
        
        Point temp;

        Point normalizedEye;

        normalizedEye.x = eye.x;
        normalizedEye.y = eye.y;
        normalizedEye.z = eye.z;

        this->normalize(&normalizedEye);

        temp.x = normalizedEye.x + lookAt.x;
        temp.y = normalizedEye.y + lookAt.y;
        temp.z = normalizedEye.z + lookAt.z;

        eye.x -= (moveRate * (up.x));
        eye.y -= (moveRate * (up.y));
        eye.z -= (moveRate * (up.z));

        //* Normalize the updated eye
        normalizedEye.x = eye.x;
        normalizedEye.y = eye.y;
        normalizedEye.z = eye.z;

        this->normalize(&normalizedEye);


        lookAt.x = temp.x - normalizedEye.x;
        lookAt.y = temp.y - normalizedEye.y;
        lookAt.z = temp.z - normalizedEye.z;
    }

    void moveDown(){
        eye.x -= (moveRate * (up.x));
        eye.y -= (moveRate * (up.y));
        eye.z -= (moveRate * (up.z));
    }


    void rotateLeft(){
        rotate(&right, &lookAt, angleRate);    
    }

    void rotateRight(){
        rotate(&right, &lookAt, -angleRate);      
    }

    void lookUp(){
        rotate(&lookAt, &up, angleRate);
    }

    void lookDown(){
        rotate(&lookAt, &up, -angleRate);
    }

    void tiltClockwise(){
        rotate(&up, &right, -angleRate);
    }

    void tiltAntiClockwise(){
        rotate(&up, &right, angleRate);
    }

};





class Ray
{
    public:
        Point start;
        Point dir;


        Ray(){
            start.x = 0;
            start.y = 0;
            start.z = 0;

            dir.x = 0;
            dir.y = 0;
            dir.z = 0;
        }

        Ray(Point start, Point dir){
            this->start.x = start.x;
            this->start.y = start.y;
            this->start.z = start.z;

            this->dir.x = dir.x;
            this->dir.y = dir.y;
            this->dir.z = dir.z;

            this->dir.normalize();
        }
};




class Object
{
    public:
        Point reference_point;
        double height, width, length;
        Color color;
        double coEfficients[4]; //* 0-ambient, 1-diffuse, 2-specular, 3-reflection coEfficients

        int shine;


        Object(){
            reference_point.x = 0;
            reference_point.y = 0;
            reference_point.z = 0;

            height = 0;
            width = 0;
            length = 0;

            color.r = 0;
            color.g = 0;
            color.b = 0;

            coEfficients[0] = 0;
            coEfficients[1] = 0;
            coEfficients[2] = 0;
            coEfficients[3] = 0;

            shine = 0;

           
        }


        virtual void draw() = 0;

        void getAmbientColor(Color *color, Color *interSectionPointColor){
            color->r = interSectionPointColor->r * coEfficients[AMB];
            color->g = interSectionPointColor->g * coEfficients[AMB];
            color->b = interSectionPointColor->b * coEfficients[AMB];
        }

        virtual double intersect(Ray *r, Color *color, int level){
            return -1.0;
        }

        void setColor(Color c){
            color.r = c.r;
            color.g = c.g;
            color.b = c.b;
        }

        void setShine(int shine){
            this->shine = shine;
        }

        void setCoEfficients(double ambient, double diffuse, double specular, double reflection){
            coEfficients[0] = ambient;
            coEfficients[1] = diffuse;
            coEfficients[2] = specular;
            coEfficients[3] = reflection;
        }

        //* getColorAt function
        virtual Color getColorAt(Point p){
            return color;
        }
};



class Sphere: public Object
{
    public:
        Sphere(Point center, double radius, Color color, double ambient, double diffuse, double specular, double reflection, int shine){
            reference_point.x = center.x;
            reference_point.y = center.y;
            reference_point.z = center.z;

            height = radius;
            width = radius;
            length = radius;

            this->color.r = color.r;
            this->color.g = color.g;
            this->color.b = color.b;

            coEfficients[0] = ambient;
            coEfficients[1] = diffuse;
            coEfficients[2] = specular;
            coEfficients[3] = reflection;

            this->shine = shine;
            
        }


        void drawSphereHelper(double radius,int sectors,int stacks){
            Point points[500][500];
            int i,j;
            double h,r;
            //generate points
            for(i=0;i<=stacks;i++)
            {
                h=radius*sin(((double)i/(double)stacks)*(M_PI/2));
                r=radius*cos(((double)i/(double)stacks)*(M_PI/2));
                for(j=0;j<=sectors;j++)
                {
                    points[i][j].x=r*cos(((double)j/(double)sectors)*2*M_PI);
                    points[i][j].y=r*sin(((double)j/(double)sectors)*2*M_PI);
                    points[i][j].z=h;
                }
            }


            for(i=0;i<stacks;i++)
            {
                
                for(j=0;j<sectors;j++)
                {
                    

                    glColor3f(color.r, color.g, color.b);
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


        void drawSphere(double radius,int sectors,int stacks){
            glPushMatrix();
            {
                
                drawSphereHelper(radius,sectors,stacks);
            }
            glPopMatrix();

            glPushMatrix();
            {
                glRotatef(180,0,1,0);
                drawSphereHelper(radius,sectors,stacks);
            }
            glPopMatrix();

        }


        virtual void draw(){

            glPushMatrix();
            {
                glTranslatef(reference_point.x, reference_point.y, reference_point.z);
                drawSphere(length, 100, 100);
            }
            glPopMatrix();
        }  


        virtual double intersect(Ray *r, Color *color, int level){

            double a, b, c;
            double t;

            //* Rd.Rd * t^2 + 2 * Rd. Ro * t + Ro . Ro - r^2 = 0;
            //* a = Rd.Rd
            //* a = 1, because Rd is normalized
            a = 1;

           
            //* At first adjust ray start point with respect to the sphere center
            Point adjustedStart;
            adjustedStart.x = r->start.x - reference_point.x;
            adjustedStart.y = r->start.y - reference_point.y;
            adjustedStart.z = r->start.z - reference_point.z;

            //* b = 2 * Rd. Ro
            b = 2 * ((r->dir.x * adjustedStart.x) + (r->dir.y * adjustedStart.y) + (r->dir.z * adjustedStart.z));

            //* c = Ro . Ro - r^2
            c = ((adjustedStart.x * adjustedStart.x) + (adjustedStart.y * adjustedStart.y) + (adjustedStart.z * adjustedStart.z)) - (length * length);


            //* Find the discriminant
            double discriminant = (b*b) - (4*a*c);

            //* If the discriminant is negative, there are no real roots
            if(discriminant < 0){
                t = -1.0;
            }

            //* If the discriminant is 0, there is one real root
            else if(discriminant == 0){
                t = -b / (2*a*1.0);
            }

            //* If the discriminant is positive, there are two real roots
            //* Choose the closest positive
            else{
                double root1 = (-b + sqrt(discriminant)) / (2*a*1.0);
                double root2 = (-b - sqrt(discriminant)) / (2*a*1.0);

                if(root1 < 0 && root2 < 0){
                    t = -1.0;
                }else if(root1 < 0){
                    t = root2;
                }else if(root2 < 0){
                    t = root1;
                }else{
                    if(root1 < root2){
                        t = root1;
                    }else{
                        t = root2;
                    }
                }
            }           

            
            if(level == 0){
                return t;
            }

            if(t < 0){
                return -1.0;
            }

    

            //^ For pointLight Sources
            //* interSectionPoint = r->start + r->dir * t
            Point interSectionPoint;
            interSectionPoint.x = r->start.x + (r->dir.x * t);
            interSectionPoint.y = r->start.y + (r->dir.y * t);
            interSectionPoint.z = r->start.z + (r->dir.z * t);

            //* interSectionPointColor = getColorAt(interSectionPoint)
            Color interSectionPointColor = getColorAt(interSectionPoint);

            //* color = interSectionPointColor * coEfficients[AMB]
            getAmbientColor(color, &interSectionPointColor);

            //* Calculate the normal at the intersection point
            Point normal;
            normal.x = interSectionPoint.x - reference_point.x;
            normal.y = interSectionPoint.y - reference_point.y;
            normal.z = interSectionPoint.z - reference_point.z;

            normal.normalize();

            //* For each point light pl in pointLights
            //*     cast ray1 from pl.light_pos to interSectionPoint

            for(int i=0; i<pointLights.size(); i++){
                PointLight *pl = pointLights[i];
                Point rayDirection ;
                rayDirection.x = interSectionPoint.x - pl->position.x;
                rayDirection.y = interSectionPoint.y - pl->position.y;
                rayDirection.z = interSectionPoint.z - pl->position.z;  
                rayDirection.normalize();

                Ray ray1(pl->position, rayDirection);

                //* if intersectionPoint is in shadow, the diffuse
                //* and specular components need not be calculated
                //* if ray1 is not obscured by any object
                bool isObscured = false;

                //* epsilon
                double epsilon = 1e-5;

                //* Calculate the length between the light source and the intersection point
                //* If the length is less than epsilon, then the light source is inside the object
                double length = sqrt((interSectionPoint.x - pl->position.x)*(interSectionPoint.x - pl->position.x) + (interSectionPoint.y - pl->position.y)*(interSectionPoint.y - pl->position.y) + (interSectionPoint.z - pl->position.z)*(interSectionPoint.z - pl->position.z));

                if(length < epsilon){
                    continue;
                }

                for(int j=0; j<objects.size(); j++){
                    double tTmp = objects[j]->intersect(&ray1, color, 0);
                    if(tTmp > 0 && (tTmp + epsilon) < length){
                        isObscured = true;
                        break;
                    }
                }


                if(!isObscured){
                    //* If ray1 is not obscured by any object
                    //* Calculate lambertValue using normal, ray1
                    double lambertValue = (normal.x * -rayDirection.x) + (normal.y * -rayDirection.y) + (normal.z * -rayDirection.z);
                    if(lambertValue < 0){
                        lambertValue = 0.0;
                    }
                    

                    //* Find reflected ray, rayr for ray1
                    Point reflectedRayDir;
                    reflectedRayDir.x = ray1.dir.x - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.x;
                    reflectedRayDir.y = ray1.dir.y - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.y;
                    reflectedRayDir.z = ray1.dir.z - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.z;

                    reflectedRayDir.normalize();
                    Ray reflectedRay(interSectionPoint, reflectedRayDir);

                    //* Calculate the phong using r, reflectedRay
                    double phong = (reflectedRayDir.x * -r->dir.x) + (reflectedRayDir.y * -r->dir.y) + (reflectedRayDir.z * -r->dir.z);
                    if(phong < 0){
                        phong = 0.0;
                    }

                    //*Update color using lambertValue, phong, pl.color, coEfficients[DIFF], coEfficients[SPEC]
                    //* color += pl.color * (coEfficients[DIFF] * lambertValue * interSectionPointColor)
                    //* color += pl.color * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor)

                    color->r += pl->color.r * (coEfficients[DIFF] * lambertValue * interSectionPointColor.r);
                    color->r += pl->color.r * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.r);

                    color->g += pl->color.g * (coEfficients[DIFF] * lambertValue * interSectionPointColor.g);
                    color->g += pl->color.g * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.g);

                    color->b += pl->color.b * (coEfficients[DIFF] * lambertValue * interSectionPointColor.b);
                    color->b += pl->color.b * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.b);

                }

            }



            //^ For spotLight Sources
            for(int i=0; i<spotLights.size(); i++){
                SpotLight *sl = spotLights[i];
                Point rayDirection ;
                rayDirection.x = interSectionPoint.x - sl->pointLight.position.x;
                rayDirection.y = interSectionPoint.y - sl->pointLight.position.y;
                rayDirection.z = interSectionPoint.z - sl->pointLight.position.z;  
                rayDirection.normalize();

                //* Don't cast the ray from light_pos to intersectionPoint
                //* if it exceeds cut-ff-angle for the light source
                
                //* dot product of rayDirection and light_direction
                double dotProduct = (rayDirection.x * sl->light_direction.x) + (rayDirection.y * sl->light_direction.y) + (rayDirection.z * sl->light_direction.z);
                //* angle = acos(dotProduct/ (|rayDirection| * |light_direction|))
                double rayDirectionLength = sqrt(rayDirection.x*rayDirection.x + rayDirection.y*rayDirection.y + rayDirection.z*rayDirection.z);
                double lightDirectionLength = sqrt(sl->light_direction.x*sl->light_direction.x + sl->light_direction.y*sl->light_direction.y + sl->light_direction.z*sl->light_direction.z);
                double angle = acos(dotProduct / (rayDirectionLength * lightDirectionLength));
                //* Convert the angle to degrees
                angle = (angle * 180) / M_PI;

                

                if(angle > sl->cutoff_angle){
                    continue;
                }

                cout << "Angle: " << angle << endl;
                cout << "Cutoff Angle: " << sl->cutoff_angle << endl;
                cout << endl;

                Ray ray1(sl->pointLight.position, rayDirection);

                //* if intersectionPoint is in shadow, the diffuse
                //* and specular components need not be calculated
                //* if ray1 is not obscured by any object
                bool isObscured = false;

                //* epsilon
                double epsilon = 1e-5;

                //* Calculate the length between the light source and the intersection point
                //* If the length is less than epsilon, then the light source is inside the object
                double length = sqrt((interSectionPoint.x - sl->pointLight.position.x)*(interSectionPoint.x - sl->pointLight.position.x) + (interSectionPoint.y - sl->pointLight.position.y)*(interSectionPoint.y - sl->pointLight.position.y) + (interSectionPoint.z - sl->pointLight.position.z)*(interSectionPoint.z - sl->pointLight.position.z));

                if(length < epsilon){
                    continue;
                }

                for(int j=0; j<objects.size(); j++){
                    double tTmp = objects[j]->intersect(&ray1, color, 0);
                    if(tTmp > 0 && (tTmp + epsilon) < length){
                        isObscured = true;
                        break;
                    }
                }


                if(!isObscured){
                    //* If ray1 is not obscured by any object
                    //* Calculate lambertValue using normal, ray1
                    double lambertValue = (normal.x * -rayDirection.x) + (normal.y * -rayDirection.y) + (normal.z * -rayDirection.z);
                    if(lambertValue < 0){
                        lambertValue = 0.0;
                    }
                    

                    //* Find reflected ray, rayr for ray1
                    Point reflectedRayDir;
                    reflectedRayDir.x = ray1.dir.x - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.x;
                    reflectedRayDir.y = ray1.dir.y - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.y;
                    reflectedRayDir.z = ray1.dir.z - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.z;

                    reflectedRayDir.normalize();
                    Ray reflectedRay(interSectionPoint, reflectedRayDir);

                    //* Calculate the phong using r, reflectedRay
                    double phong = (reflectedRayDir.x * -r->dir.x) + (reflectedRayDir.y * -r->dir.y) + (reflectedRayDir.z * -r->dir.z);
                    if(phong < 0){
                        phong = 0.0;
                    }

                    //*Update color using lambertValue, phong, pl.color, coEfficients[DIFF], coEfficients[SPEC]
                    //* color += pl.color * (coEfficients[DIFF] * lambertValue * interSectionPointColor)
                    //* color += pl.color * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor)

                    color->r += sl->pointLight.color.r * (coEfficients[DIFF] * lambertValue * interSectionPointColor.r);
                    color->r += sl->pointLight.color.r * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.r);

                    color->g += sl->pointLight.color.g * (coEfficients[DIFF] * lambertValue * interSectionPointColor.g);
                    color->g += sl->pointLight.color.g * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.g);

                    color->b += sl->pointLight.color.b * (coEfficients[DIFF] * lambertValue * interSectionPointColor.b);
                    color->b += sl->pointLight.color.b * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.b);
                }

            }

            return t;

        }        

};




class Triangle : public Object
{
    public:
        Point a, b, c;

        Triangle(Point a, Point b, Point c, Color color, double ambient, double diffuse, double specular, double reflection, int shine){
            this->a.x = a.x;
            this->a.y = a.y;
            this->a.z = a.z;

            this->b.x = b.x;
            this->b.y = b.y;
            this->b.z = b.z;

            this->c.x = c.x;
            this->c.y = c.y;
            this->c.z = c.z;

            this->color.r = color.r;
            this->color.g = color.g;
            this->color.b = color.b;

            coEfficients[0] = ambient;
            coEfficients[1] = diffuse;
            coEfficients[2] = specular;
            coEfficients[3] = reflection;

            this->shine = shine;
        }

        virtual void draw(){
            glPushMatrix();
            {
                glBegin(GL_TRIANGLES);{
                    glColor3f(color.r, color.g, color.b);
                    glVertex3f(a.x, a.y, a.z);
                    glVertex3f(b.x, b.y, b.z);
                    glVertex3f(c.x, c.y, c.z);
                }glEnd();
            }
            glPopMatrix();
        }
};




class General : public Object
{
    public:
        double A, B, C, D, E, F, G, H, I, J;

        General(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J, Color color, double ambient, double diffuse, double specular, double reflection, int shine){
            this->A = A;
            this->B = B;
            this->C = C;
            this->D = D;
            this->E = E;
            this->F = F;
            this->G = G;
            this->H = H;
            this->I = I;
            this->J = J;

            this->color.r = color.r;
            this->color.g = color.g;
            this->color.b = color.b;

            coEfficients[0] = ambient;
            coEfficients[1] = diffuse;
            coEfficients[2] = specular;
            coEfficients[3] = reflection;

            this->shine = shine;
        }

        virtual void draw(){
            //* Do Nothing
            return;
        }
};








class Floor : public Object
{
    public:
        double floorWidth, tileWidth;
        
        Floor(double floorWidth, double tileWidth){
            this->floorWidth = floorWidth;
            this->tileWidth = tileWidth;

            reference_point.x = -floorWidth/2;
            reference_point.y = -floorWidth/2;
            reference_point.z = 0;
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


        void drawChekerBoard(){
            
            for(int i=-floorWidth/2,k=0; i<floorWidth/2 + 1; i+=tileWidth,k++){
                for(int j=-floorWidth/2,l=0; j<floorWidth/2 +1; j+=tileWidth,l++){
                    if((k+l)%2 == 0){
                        glPushMatrix();
                        {
                            glTranslatef(i, j, 0);
                            drawBox(tileWidth, 0);
                        }
                        glPopMatrix();
                    }else{
                        glPushMatrix();
                        {
                            glTranslatef(i, j, 0);
                            drawBox(tileWidth, 1);
                        }
                        glPopMatrix();
                    }
                }
            }
        }

        virtual void draw(){
            glPushMatrix();
            {
                drawChekerBoard();
            }
            glPopMatrix();
        }


        virtual Color getColorAt(Point p){
            int i = (p.x - reference_point.x) / tileWidth;
            int j = (p.y - reference_point.y) / tileWidth;

            //* If the point is outside the floor
            if(i < 0 || j < 0 || i > floorWidth/tileWidth || j > floorWidth/tileWidth){
                return Color(0, 0, 0);
            }

            if((i+j)%2 == 0){
                return Color(0, 0, 0);
            }else{
                return Color(1, 1, 1);
            }
        }


        virtual double intersect(Ray *r, Color *color, int level){

            Point normal;
            normal.x = 0;
            normal.y = 0;
            normal.z = 1;

            double t = -1.0;

            //* If the ray is parallel to the floor
            //* Then the ray will never intersect the floor
            //* Dot product of the ray direction and the normal will be 0
            if((r->dir.x * normal.x + r->dir.y * normal.y + r->dir.z * normal.z) * 1000 == 0){
                return -1.0;
            }

            //* t = -(ray.start * normal) / (ray.dir . normal)
            t = -((r->start.x * normal.x + r->start.y * normal.y + r->start.z * normal.z) / (r->dir.x * normal.x + r->dir.y * normal.y + r->dir.z * normal.z));

            //* point = ray.start + t * ray.dir
            Point point;
            point.x = r->start.x + t * r->dir.x;
            point.y = r->start.y + t * r->dir.y;
            point.z = r->start.z + t * r->dir.z;

            //* If the point is outside the floor
            if(point.x <= reference_point.x || point.x >= abs(reference_point.x) && point.y <= reference_point.y && point.y >= abs(reference_point.y)){
                return -1.0;
            }

            if(level == 0){
                return t;
            }


            if(t < 0){
                return -1.0;
            }


    

            //^ For pointLight Sources
            //* interSectionPoint = r->start + r->dir * t
            Point interSectionPoint;
            interSectionPoint.x = r->start.x + (r->dir.x * t);
            interSectionPoint.y = r->start.y + (r->dir.y * t);
            interSectionPoint.z = r->start.z + (r->dir.z * t);

            //* interSectionPointColor = getColorAt(interSectionPoint)
            Color interSectionPointColor = getColorAt(interSectionPoint);
          
            //* color = interSectionPointColor * coEfficients[AMB]
            getAmbientColor(color, &interSectionPointColor);

            

            normal.normalize();

            //* For each point light pl in pointLights
            //*     cast ray1 from pl.light_pos to interSectionPoint

            for(int i=0; i<pointLights.size(); i++){
                PointLight *pl = pointLights[i];
                Point rayDirection ;
                rayDirection.x = interSectionPoint.x - pl->position.x;
                rayDirection.y = interSectionPoint.y - pl->position.y;
                rayDirection.z = interSectionPoint.z - pl->position.z;  
                rayDirection.normalize();

                Ray ray1(pl->position, rayDirection);

                //* if intersectionPoint is in shadow, the diffuse
                //* and specular components need not be calculated
                //* if ray1 is not obscured by any object
                bool isObscured = false;

                //* epsilon
                double epsilon = 1e-5;

                //* Calculate the length between the light source and the intersection point
                //* If the length is less than epsilon, then the light source is inside the object
                double length = sqrt((interSectionPoint.x - pl->position.x)*(interSectionPoint.x - pl->position.x) + (interSectionPoint.y - pl->position.y)*(interSectionPoint.y - pl->position.y) + (interSectionPoint.z - pl->position.z)*(interSectionPoint.z - pl->position.z));

                if(length < epsilon){
                    continue;
                }

                for(int j=0; j<objects.size(); j++){
                    double tTmp = objects[j]->intersect(&ray1, color, 0);
                    if(tTmp > 0 && (tTmp + epsilon) < length){
                        isObscured = true;
                        break;
                    }
                }


                if(!isObscured){
                    //* If ray1 is not obscured by any object
                    //* Calculate lambertValue using normal, ray1
                    double lambertValue = (normal.x * -rayDirection.x) + (normal.y * -rayDirection.y) + (normal.z * -rayDirection.z);
                    if(lambertValue < 0){
                        lambertValue = 0.0;
                    }
                    

                    //* Find reflected ray, rayr for ray1
                    Point reflectedRayDir;
                    reflectedRayDir.x = ray1.dir.x - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.x;
                    reflectedRayDir.y = ray1.dir.y - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.y;
                    reflectedRayDir.z = ray1.dir.z - 2 * (normal.x * ray1.dir.x + normal.y * ray1.dir.y + normal.z * ray1.dir.z) * normal.z;

                    reflectedRayDir.normalize();
                    Ray reflectedRay(interSectionPoint, reflectedRayDir);

                    //* Calculate the phong using r, reflectedRay
                    double phong = (reflectedRayDir.x * -r->dir.x) + (reflectedRayDir.y * -r->dir.y) + (reflectedRayDir.z * -r->dir.z);
                    if(phong < 0){
                        phong = 0.0;
                    }

                    //*Update color using lambertValue, phong, pl.color, coEfficients[DIFF], coEfficients[SPEC]
                    //* color += pl.color * (coEfficients[DIFF] * lambertValue * interSectionPointColor)
                    //* color += pl.color * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor)

                    color->r += pl->color.r * (coEfficients[DIFF] * lambertValue * interSectionPointColor.r);
                    color->r += pl->color.r * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.r);

                    color->g += pl->color.g * (coEfficients[DIFF] * lambertValue * interSectionPointColor.g);
                    color->g += pl->color.g * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.g);

                    color->b += pl->color.b * (coEfficients[DIFF] * lambertValue * interSectionPointColor.b);
                    color->b += pl->color.b * (coEfficients[SPEC] * pow(phong, shine) * interSectionPointColor.b);

                }

            }

            return t;

        }
};










