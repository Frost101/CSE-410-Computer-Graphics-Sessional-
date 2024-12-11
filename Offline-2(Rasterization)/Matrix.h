#ifndef MATRIX_H
#define MATRIX_H


#include<bits/stdc++.h>
#include "Point.h"
#define PI acos(-1)
using namespace std;


class Matrix{
    public:
        double mat[4][4];
        Matrix(){
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    mat[i][j] = 0;
                }
            }
        }


        void setMatrix(double m[4][4]){
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    mat[i][j] = m[i][j];
                }
            }
        }


        Matrix operator*(Matrix m){
            Matrix result;
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    for(int k=0; k<4; k++){
                        result.mat[i][j] += mat[i][k] * m.mat[k][j];
                    }
                }
            }
            return result;
        }



        Point operator*(Point p){
            double arr[4][1];
            arr[0][0] = p.x, arr[1][0] = p.y, arr[2][0] = p.z, arr[3][0] = p.w;
            double res[4][1];
            for(int i=0; i<4; i++){
                for(int j=0; j<1; j++){
                    res[i][j] = 0;
                    for(int k=0; k<4; k++){
                        res[i][j] += mat[i][k] * arr[k][j];
                    }
                }
            }
            Point result(res[0][0], res[1][0], res[2][0], res[3][0]);
            result.homogenize();        //? Make sure that w = 1.0
            return result;
        }


        Matrix operator+(Matrix m){
            Matrix result;
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    result.mat[i][j] = mat[i][j] + m.mat[i][j];
                }
            }
            return result;
        }


        Matrix operator-(Matrix m){
            Matrix result;
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    result.mat[i][j] = mat[i][j] - m.mat[i][j];
                }
            }
            return result;
        }


        void print(){
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    cout << mat[i][j] << " ";
                }
                cout << endl;
            }
        }


        void setIdentity(){
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    if(i == j) mat[i][j] = 1;
                    else mat[i][j] = 0;
                }
            }
        }


        void setTranslate(double tx, double ty, double tz){
            setIdentity();
            mat[0][3] = tx;
            mat[1][3] = ty;
            mat[2][3] = tz;
        }


        void setScale(double sx, double sy, double sz){
            setIdentity();
            mat[0][0] = sx;
            mat[1][1] = sy;
            mat[2][2] = sz;
        }


        Point rodrigues(Point P1, Point P2, double angle){
            Point result;

            angle = (angle *  PI) / 180.0;

            result = (P1 * cos(angle)) + (P2 * ((P1.dotProduct(P2)) * (1 - cos(angle)))) + (P2.crossProduct(P1)) * sin(angle);

            return result;
        }


        void setRotate(double angle, double ax, double ay, double az){
            //* normalize the axis vector
            Point a(ax, ay, az);
            a.normalize();

            //* Rodrigues' Rotation Formula
            Point C1 = rodrigues(Point(1, 0, 0), a, angle);
            Point C2 = rodrigues(Point(0, 1, 0), a, angle);
            Point C3 = rodrigues(Point(0, 0, 1), a, angle);

            setIdentity();
            mat[0][0] = C1.x, mat[1][0] = C1.y, mat[2][0] = C1.z;
            mat[0][1] = C2.x, mat[1][1] = C2.y, mat[2][1] = C2.z;
            mat[0][2] = C3.x, mat[1][2] = C3.y, mat[2][2] = C3.z;
        }


};


#endif