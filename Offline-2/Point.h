#ifndef POINT_H
#define POINT_H


#include<bits/stdc++.h>
using namespace std;

class Point{
    public:
        double x, y, z, w;
        Point(){
            x = 0;
            y = 0;
            z = 0;
            w = 1.0;
        }


        Point(double x, double y, double z){
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = 1.0;
        }


        Point(double x, double y, double z, double w){
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }


        Point operator+(Point p){
            return Point(this->x + p.x, this->y + p.y, this->z + p.z);
        }


        Point operator-(Point p){
            return Point(this->x - p.x, this->y - p.y, this->z - p.z);
        }


        double dotProduct(Point p){
            return this->x * p.x + this->y * p.y + this->z * p.z;
        }


        Point operator*(double f){
            return Point(this->x * f, this->y * f, this->z * f);
        }

        void normalize(){
            double len = sqrt(x*x + y*y + z*z);
            x /= len;
            y /= len;
            z /= len;
        }

        void homogenize(){
            x /= w;
            y /= w;
            z /= w;
            w  = 1.0;
        }

        Point crossProduct(Point p){
            return Point(this->y * p.z - this->z * p.y, this->z * p.x - this->x * p.z, this->x * p.y - this->y * p.x);
        }
        void print(){
            cout << x << " " << y << " " << z << endl;
        }
};

#endif