#include<bits/stdc++.h>
#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include<windows.h>
    #include<glut.h>
#endif
using namespace std;
struct BoundaryPoint
{
    double x;
    double y;
    double z;
};


class BoundaryPlanes{
    public:
        BoundaryPoint P1;
        BoundaryPoint P2;
        BoundaryPoint P3;
        BoundaryPoint normal;
        BoundaryPlanes(BoundaryPoint P1, BoundaryPoint P2, BoundaryPoint P3){
            this->P1 = P1;
            this->P2 = P2;
            this->P3 = P3;
            this->normal = getNormal();
        }

        BoundaryPoint getNormal(){
            BoundaryPoint v1,v2;
            v1.x = P2.x - P1.x;
            v1.y = P2.y - P1.y;
            v1.z = P2.z - P1.z;

            v2.x = P3.x - P1.x;
            v2.y = P3.y - P1.y;
            v2.z = P3.z - P1.z;

            BoundaryPoint normal;
            normal.x = v1.y * v2.z - v1.z * v2.y;
            normal.y = v1.z * v2.x - v1.x * v2.z;
            normal.z = v1.x * v2.y - v1.y * v2.x;


            // cout << "v1: " << v1.x << " " << v1.y << " " << v1.z << endl;
            // cout << "v2: " << v2.x << " " << v2.y << " " << v2.z << endl;
            // cout << normal.x << " " << normal.y << " " << normal.z << endl;
            // cout << "-------------------" << endl;

            return normal;
        }

};



class Boundary{
    public:
        int n;
        vector<BoundaryPoint> points;  
        vector<BoundaryPlanes> planes;
        double height;    
        bool flag = true;
        Boundary(double height){
            n = 0;
            this->height = height;
        }

        void addPoint(double x, double y){
            BoundaryPoint p;
            p.x = x;
            p.y = y;
            points.push_back(p);
            n++;
        }

        void draw(){
            int i;
            for(i=0;i<n;i++)
            {
                glBegin(GL_QUADS);{
                    glColor3f(1,0,0);
                    glVertex3f(points[i].x,points[i].y,0);
                    glVertex3f(points[(i+1)%n].x,points[(i+1)%n].y,0);
                    glVertex3f(points[(i+1)%n].x,points[(i+1)%n].y,height);
                    glVertex3f(points[i].x,points[i].y,height);

                    BoundaryPoint P1,P2,P3;
                    P1.x = points[i].x;
                    P1.y = points[i].y;
                    P1.z = 0;

                    P2.x = points[(i+1)%n].x;
                    P2.y = points[(i+1)%n].y;
                    P2.z = 0;

                    P3.x = points[i].x;
                    P3.y = points[i].y;
                    P3.z = height;

                    BoundaryPlanes plane(P1,P2,P3);

                    if(flag)
                    planes.push_back(plane);
                }glEnd();
            }
            flag = false;
        }


        double calculateDistance(double x, double y, double z, double radius){
            for(int i=0;i<planes.size();i++){
                BoundaryPlanes plane = planes[i];
                double d = (plane.normal.x * (x - plane.P1.x) + plane.normal.y * (y - plane.P1.y) + plane.normal.z * (z - plane.P1.z))/sqrt(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z);

                return abs(d);
                  
            }
        }


        bool checkCollision(double x, double y, double z, double radius){
            
            for(int i=0;i<planes.size();i++){
                BoundaryPlanes plane = planes[i];
                double d = (plane.normal.x * (x - plane.P1.x) + plane.normal.y * (y - plane.P1.y) + plane.normal.z * (z - plane.P1.z))/sqrt(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z);
               
                if(abs(d) <= radius){
                    return true;
                }
            }
            return false;
        }

        void updateBallDirectionAfterCollision(double posX, double posY, double posZ, double &dirX, double &dirY, double &dirZ, double radius){
            for(int i=0;i<planes.size();i++){
                BoundaryPlanes plane = planes[i];
                double d = (plane.normal.x * (posX - plane.P1.x) + plane.normal.y * (posY - plane.P1.y) + plane.normal.z * (posZ - plane.P1.z))/sqrt(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z);
               
                if(abs(d) <= radius){
                    //* Normalize the normal vector
                    double magnitudeNormal = sqrt(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z);
                    double dotProduct = (dirX * plane.normal.x + dirY * plane.normal.y + (dirZ) * plane.normal.z) / magnitudeNormal;
                    dirX = dirX - (2 * dotProduct * plane.normal.x)/magnitudeNormal;
                    dirY = dirY - (2 * dotProduct * plane.normal.y)/magnitudeNormal;
                    dirZ = 0.0; //* Only xy plane e move korbe
                    break;
                }
            }
        }


        vector<double> measureDistanceFromBallToWall(double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double radius){
            vector<double> distances;
            for(int i=0;i<planes.size();i++){
                BoundaryPlanes plane = planes[i];
                double d = (plane.normal.x * (posX - plane.P1.x) + plane.normal.y * (posY - plane.P1.y) + plane.normal.z * (posZ - plane.P1.z))/sqrt(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z);
               
                //* Angle Between the normal vector and ball's direction vector
                double angle = acos((dirX * plane.normal.x + dirY * plane.normal.y + (dirZ) * plane.normal.z)/(sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ) * sqrt(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z)));
                cout << "Angle in degree: " << angle * 180 / 3.1416 << endl;
                double distance = d/cos(angle);
                distances.push_back(abs(distance)-radius);
                cout << "Distance: " << distance << endl;
            }
            cout << "-------------------" << endl;
            return distances;
        }




        // void calculateDistanceFromBallToWall(BoundaryPlanes plane, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double radius){
        //     double d = (plane.normal.x * (posX - plane.P1.x) + plane.normal.y * (posY - plane.P1.y) + plane.normal.z * (posZ - plane.P1.z))/sqrt(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z);
           
        //     //* Angle Between the normal vector and ball's direction vector
        //     double angle = acos((dirX * plane.normal.x + dirY * plane.normal.y + (dirZ) * plane.normal.z)/(sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ) * sqrt(plane.normal.x * plane.normal.x + plane.normal.y * plane.normal.y + plane.normal.z * plane.normal.z)));
        //     double distance = d/cos(angle);
        //     cout << "Distance: " << distance << endl;
        // }
        
};