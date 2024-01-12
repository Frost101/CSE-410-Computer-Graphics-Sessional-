#include<bits/stdc++.h>
#include "Matrix.h"
#include "Triangle.h"
#include "Point.h"
using namespace std;


int main(){
    string input_stage1 = "scene.txt";
    string output_stage1 = "stage1.txt";


    ifstream fin(input_stage1);
    ofstream fout(output_stage1);

    //* Scene Description
    //* Line:01: eyeX eyeY eyeZ
    //* Line:02: lookX lookY lookZ
    //* Line:03: upX upY upZ                //For gluLookAt function
    //* Line:04: fovY aspectRatio near far  //For gluPerspective function

    double eyeX, eyeY, eyeZ;
    double lookX, lookY, lookZ;
    double upX, upY, upZ;
    double fovY, aspectRatio, near, far;

    fin >> eyeX >> eyeY >> eyeZ;
    fin >> lookX >> lookY >> lookZ;
    fin >> upX >> upY >> upZ;
    fin >> fovY >> aspectRatio >> near >> far;


    Point eye(eyeX, eyeY, eyeZ);
    Point look(lookX, lookY, lookZ);
    Point up(upX, upY, upZ);


    //* Stage:01: Modeling Transformation
    stack<Matrix> stk;
    Matrix currentMatrix;
    currentMatrix.setIdentity();
    
    while(!fin.eof()){
        string type;
        fin >> type;
        if(type == "triangle" ){
            double x1, y1, z1;
            double x2, y2, z2;
            double x3, y3, z3;
            fin >> x1 >> y1 >> z1;
            fin >> x2 >> y2 >> z2;
            fin >> x3 >> y3 >> z3;
            Point p1(x1, y1, z1);
            Point p2(x2, y2, z2);
            Point p3(x3, y3, z3);
            Triangle t(p1, p2, p3);

            Point p1_new = currentMatrix * p1;
            Point p2_new = currentMatrix * p2;
            Point p3_new = currentMatrix * p3;

            fout << setprecision(7) << fixed;
            fout << p1_new.x << " " << p1_new.y << " " << p1_new.z << endl;
            fout << p2_new.x << " " << p2_new.y << " " << p2_new.z << endl;
            fout << p3_new.x << " " << p3_new.y << " " << p3_new.z << endl;
            fout << endl;
        }
        else if(type == "translate"){
            double tx, ty, tz;
            fin >> tx >> ty >> tz;

            Matrix translateMatrix;
            translateMatrix.setTranslate(tx, ty, tz);
            currentMatrix = currentMatrix * translateMatrix;
        }
        else if(type == "scale"){
            double sx, sy, sz;
            fin >> sx >> sy >> sz;

            Matrix scaleMatrix;
            scaleMatrix.setScale(sx, sy, sz);
            currentMatrix = currentMatrix * scaleMatrix;
        }
        else if(type == "rotate"){
            double angle, ax, ay, az;
            fin >> angle >> ax >> ay >> az;
            
            Matrix rotateMatrix;
            rotateMatrix.setRotate(angle, ax, ay, az);
            currentMatrix = currentMatrix * rotateMatrix;
        }
        else if(type == "push"){
            stk.push(currentMatrix);
        }
        else if(type == "pop"){
            currentMatrix = stk.top();
            stk.pop();
        }
        else if(type == "end"){
            break;
        }
    }

    fin.close();
    fout.close();



    //* Stage:02: Viewing Transformation

    Point l = look - eye;
    l.normalize();
    Point r = l.crossProduct(up);
    r.normalize();
    Point u = r.crossProduct(l);
    u.normalize();


    Matrix T;
    T.setIdentity();
    T.mat[0][3] = -eye.x;
    T.mat[1][3] = -eye.y;
    T.mat[2][3] = -eye.z;

    Matrix R;
    R.setIdentity();
    R.mat[0][0] = r.x, R.mat[0][1] = r.y, R.mat[0][2] = r.z;
    R.mat[1][0] = u.x, R.mat[1][1] = u.y, R.mat[1][2] = u.z;
    R.mat[2][0] = -l.x, R.mat[2][1] = -l.y, R.mat[2][2] = -l.z;

    Matrix V = R * T;

    fin.open("stage1.txt");
    fout.open("stage2.txt");

    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;
    while(fin >> x1){
        fin >> y1 >> z1;
        fin >> x2 >> y2 >> z2;
        fin >> x3 >> y3 >> z3;
        //* Ignore the next blank line
        string blank;
        getline(fin, blank);

        Point p1(x1, y1, z1);
        Point p2(x2, y2, z2);
        Point p3(x3, y3, z3);

        Point p1_new = V * p1;
        Point p2_new = V * p2;
        Point p3_new = V * p3;

        fout << setprecision(7) << fixed;
        fout << p1_new.x << " " << p1_new.y << " " << p1_new.z << endl;
        fout << p2_new.x << " " << p2_new.y << " " << p2_new.z << endl;
        fout << p3_new.x << " " << p3_new.y << " " << p3_new.z << endl;
        fout << endl;

        if(fin.eof()){
            break;
        }
    }

    fin.close();
    fout.close();




    //* Stage:03: Projection Transformation
    double fovX = fovY * aspectRatio;
    double t = near * tan((fovY * PI) / 360);
    double rr = near * tan((fovX * PI) / 360);

    Matrix P;
    P.setIdentity();
    P.mat[0][0] = near / rr;
    P.mat[1][1] = near / t;
    P.mat[2][2] = -(far + near) / (far - near);
    P.mat[2][3] = -(2 * far * near) / (far - near);
    P.mat[3][2] = -1;
    P.mat[3][3] = 0;

    fin.open("stage2.txt");
    fout.open("stage3.txt");

    while(fin >> x1){
        fin >> y1 >> z1;
        fin >> x2 >> y2 >> z2;
        fin >> x3 >> y3 >> z3;
        //* Ignore the next blank line
        string blank;
        getline(fin, blank);

        Point p1(x1, y1, z1);
        Point p2(x2, y2, z2);
        Point p3(x3, y3, z3);

        Point p1_new = P * p1;
        Point p2_new = P * p2;
        Point p3_new = P * p3;

        fout << setprecision(7) << fixed;
        fout << p1_new.x << " " << p1_new.y << " " << p1_new.z << endl;
        fout << p2_new.x << " " << p2_new.y << " " << p2_new.z << endl;
        fout << p3_new.x << " " << p3_new.y << " " << p3_new.z << endl;
        fout << endl;

        if(fin.eof()){
            break;
        }
    }

    fin.close();
    fout.close();
}