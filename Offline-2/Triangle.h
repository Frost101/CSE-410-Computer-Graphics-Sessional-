
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include<bits/stdc++.h>
#include "Point.h"

using namespace std;

class Triangle{
    public:
        Point p1, p2, p3;
        Triangle(){
            p1 = Point();
            p2 = Point();
            p3 = Point();
        }
        Triangle(Point p1, Point p2, Point p3){
            this->p1 = p1;
            this->p2 = p2;
            this->p3 = p3;
        }

        void setTriangle(Point p1, Point p2, Point p3){
            this->p1 = p1;
            this->p2 = p2;
            this->p3 = p3;
        }

        void print(){
            cout << "Triangle" << endl;
            p1.print();
            p2.print();
            p3.print();
        }
};

#endif



