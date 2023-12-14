
using namespace std;

struct Point
{
    double x;
    double y;
    double z;
};



class Camera
{
private:

    void rotate(Point *first, Point *second, double angleRate){
        // cout << angleRate << endl;
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


        

        // first->x = first->x * cos(angleRate) + second->x * sin(angleRate);
        // first->y = first->y * cos(angleRate) + second->y * sin(angleRate);
        // first->z = first->z * cos(angleRate) + second->z * sin(angleRate);

    
        // second->x = second->x * cos(angleRate) - first->x * sin(angleRate);
        // second->y = second->y * cos(angleRate) - first->y * sin(angleRate);
        // second->z = second->z * cos(angleRate) - first->z * sin(angleRate);

    }
    
public:
    Point eye;
    Point lookAt;
    Point up;
    Point right;

    double angleRate;
    double moveRate;

    Camera(){
        eye.x = 10;
        eye.y = 10;
        eye.z = 10;

        lookAt.x = -10;
        lookAt.y = -10;
        lookAt.z = -10;

        up.x = 0;
        up.y = 0;
        up.z = 1;

        right.x = -10;
        right.y = 10;
        right.z = 0;

        angleRate = 0.05;
        moveRate = 0.1;
    }

    void setAngleRate(double rate){
        angleRate = rate;
    }

    void setMoveRate(double rate){
        moveRate = rate;
    }

    void setCamera(){
        cout << "Camera: " << endl;
        cout << "Eye: " << eye.x << " " << eye.y << " " << eye.z << endl;
        cout << "LookAt: " << lookAt.x << " " << lookAt.y << " " << lookAt.z << endl;
        cout << "Up: " << up.x << " " << up.y << " " << up.z << endl;
        cout << "Right: " << right.x << " " << right.y << " " << right.z << endl;
        cout << "--------------" << endl;
        gluLookAt(eye.x, eye.y, eye.z, 
                eye.x + lookAt.x, eye.y + lookAt.y, eye.z + lookAt.z,
                up.x, up.y, up.z);
    }

    void setLookAt(Point p){
        lookAt.x = p.x;
        lookAt.y = p.y;
        lookAt.z = p.z;
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
    }

    void setUp(Point p){
        up.x = p.x;
        up.y = p.y;
        up.z = p.z;
    }

    void moveForward(){
        //* Normalizing the lookAt vector
        double length = sqrt(lookAt.x*lookAt.x + lookAt.y*lookAt.y + lookAt.z*lookAt.z);

        eye.x += (moveRate * (lookAt.x))/length;
        eye.y += (moveRate * (lookAt.y))/length;
        eye.z += (moveRate * (lookAt.z))/length;
    }

    void moveBackward(){
        //* Normalizing the lookAt vector
        double length = sqrt(lookAt.x*lookAt.x + lookAt.y*lookAt.y + lookAt.z*lookAt.z);

        eye.x -= (moveRate * (lookAt.x))/length;
        eye.y -= (moveRate * (lookAt.y))/length;
        eye.z -= (moveRate * (lookAt.z))/length;
    }

    void moveRight(){
        //* Normalizing the right vector
        double length = sqrt(right.x*right.x + right.y*right.y + right.z*right.z);

        eye.x += (moveRate * (right.x))/length;
        eye.y += (moveRate * (right.y))/length;
        eye.z += (moveRate * (right.z))/length;
    }

    void moveLeft(){
        //* Normalizing the right vector
        double length = sqrt(right.x*right.x + right.y*right.y + right.z*right.z);

        eye.x -= (moveRate * (right.x))/length;
        eye.y -= (moveRate * (right.y))/length;
        eye.z -= (moveRate * (right.z))/length;

    }

    void moveUp(){
        //* Normalizing the up vector
        double length = sqrt(up.x*up.x + up.y*up.y + up.z*up.z);

        eye.x += (moveRate * (up.x))/length;
        eye.y += (moveRate * (up.y))/length;
        eye.z += (moveRate * (up.z))/length;
    }

    void moveDown(){
        //* Normalizing the up vector
        double length = sqrt(up.x*up.x + up.y*up.y + up.z*up.z);

        eye.x -= (moveRate * (up.x))/length;
        eye.y -= (moveRate * (up.y))/length;
        eye.z -= (moveRate * (up.z))/length;
    }


    void rotateLeft(){
        rotate(&right, &lookAt, -angleRate);    
    }

    void rotateRight(){
        rotate(&right, &lookAt, angleRate);    
    }

    void lookUp(){
        rotate(&lookAt, &up, -angleRate);
    }

    void lookDown(){
        rotate(&lookAt, &up, angleRate);
    }

    void tiltClockwise(){
        rotate(&up, &right, -angleRate);
    }

    void tiltAntiClockwise(){
        rotate(&up, &right, angleRate);
    }

};

