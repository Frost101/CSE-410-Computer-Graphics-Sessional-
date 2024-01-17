
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
        rotate(&right, &lookAt, -angleRate);    
    }

    void rotateRight(){
        rotate(&right, &lookAt, angleRate);    
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

