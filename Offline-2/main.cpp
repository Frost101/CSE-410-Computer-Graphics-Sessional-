#include<bits/stdc++.h>
#include "Matrix.h"
#include "Triangle.h"
#include "Point.h"
#include "helper.h"
#include "bitmap_image.hpp"
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




    //* Stage:04: Clipping and scan conversion using Z-buffer algorithm
    fin.open("config.txt");
    fout.open("z_buffer.txt");

    int screen_width, screen_height;
    fin >> screen_width >> screen_height;

    fin.close();
    fin.open("stage3.txt");

    bitmap_image image(screen_width, screen_height);
    image.set_all_channels(0, 0, 0);

    double right_limit = 1.0, left_limit = -1.0;
    double top_limit = 1.0, bottom_limit = -1.0;
    double dx = (right_limit - left_limit) / screen_width;
    double dy = (top_limit - bottom_limit) / screen_height;

    double top_Y = top_limit - (dy / 2.0);
    double bottom_Y = bottom_limit + (dy / 2.0);

    double left_X = left_limit + (dx / 2.0);
    double right_X = right_limit - (dx / 2.0);

    double Z_MAX = 1.0, Z_MIN = -1.0;

    vector<vector<double>> z_buffer(screen_height, vector<double>(screen_width, Z_MAX));


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

        int r = random() % 256;
        int g = random() % 256;
        int b = random() % 256;

        vector<Point> points;
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);

        //* Apply procedure
        /*
            Pseudocode:
                for each object : Triangles
                    Find top_scanline and bottom_scanline after necessary clipping

                    for row_no from top_scanline to bottom_scanline
                        Find left_intersecting_column and right_intersecting_column
                        after necessary clipping

                        for col_no from left_intersecting_column to right_intersecting_column
                            Calculate z values
                            Compare with z-buffer and z_front_limit and update if required
                            Update pixel information if required
                        end
                    end
                end
        */

        //* Find top_scanline and bottom_scanline after necessary clipping
        double top_scanline = min(top_Y, max(p1.y, max(p2.y, p3.y)));
        double bottom_scanline = max(bottom_Y, min(p1.y, min(p2.y, p3.y)));

        top_scanline = (int) ceil((top_Y - top_scanline) / dy);
        bottom_scanline = (int) floor((top_Y - bottom_scanline) / dy);


        if(bottom_scanline >= screen_height){
            bottom_scanline = screen_height - 1;
        }

        for(int row_no = top_scanline; row_no <= bottom_scanline; row_no++){
            double y_val = top_Y - (row_no * dy);

            //* Find left_intersecting_column and right_intersecting_column after necessary clipping
            vector<double> z_vals, x_vals;

            for(int i=0; i<3; i++){
                int j = (i + 1) % 3;
                
                Point temp_p1 = points[i];
                Point temp_p2 = points[j];
                
                double smaller_y = min(temp_p1.y, temp_p2.y);
                double greater_y = max(temp_p1.y, temp_p2.y);

                cout << smaller_y << " " << greater_y << " " << y_val << endl;

                if(temp_p1.y == temp_p2.y){
                    continue;
                }
                else if(y_val >= smaller_y && y_val <= greater_y){
                    double x_val = (y_val - temp_p1.y) * (temp_p2.x - temp_p1.x) / (temp_p2.y - temp_p1.y) + temp_p1.x;
                    double z_val = (y_val - temp_p1.y) * (temp_p2.z - temp_p1.z) / (temp_p2.y - temp_p1.y) + temp_p1.z;

                    x_vals.push_back(x_val);
                    z_vals.push_back(z_val);        
                }

                if(x_vals.size() == 2){
                    break;
                }
            }

             


            vector<double> temp_x_vals = x_vals;
            //* Clipping
            x_vals[0] = max(left_X, min(x_vals[0], right_X));
            x_vals[1] = max(left_X, min(x_vals[1], right_X));

            //* Adjust z_vals accordingly
            if(temp_x_vals[0] != x_vals[0]){
                z_vals[0] = z_vals[0] + (z_vals[1] - z_vals[0]) * (x_vals[0] - temp_x_vals[0]) / (temp_x_vals[1] - temp_x_vals[0]);
            }
            if(temp_x_vals[1] != x_vals[1]){
                z_vals[1] = z_vals[0] + (z_vals[1] - z_vals[0]) * (x_vals[1] - temp_x_vals[0]) / (temp_x_vals[1] - temp_x_vals[0]);
            }

            if(x_vals[0] > x_vals[1]){
                swap(x_vals[0], x_vals[1]);
                swap(z_vals[0], z_vals[1]);
            }

            int left_intersecting_column = (int) round((x_vals[0] - left_X) / dx);
            int right_intersecting_column = (int) round((x_vals[1] - left_X) / dx);

            for(int col_no = left_intersecting_column; col_no <= right_intersecting_column; col_no++){
                double x_val = left_X + (col_no * dx);

                //* Calculate z values
                double z_val = z_vals[0] + (z_vals[1] - z_vals[0]) * (x_val - x_vals[0]) / (x_vals[1] - x_vals[0]);

                //* Compare with z-buffer and z_front_limit and update if required
                if(z_val < z_buffer[row_no][col_no] && z_val >= Z_MIN){
                    z_buffer[row_no][col_no] = z_val;
                    image.set_pixel(col_no, row_no, r, g, b);
                }
            }
        }
    }

    image.save_image("out.bmp");

    fin.close();

    for(int i=0; i<screen_height; i++){
        for(int j=0; j<screen_width; j++){
            if(z_buffer[i][j] < Z_MAX){
                fout << setprecision(6) << fixed;
                fout << z_buffer[i][j] << "\t";
            }
            
        }
        fout << endl;
    }

    fout.close();

    //* Free memory
    for(int i=0; i<screen_height; i++){
        z_buffer[i].clear();
    }
    z_buffer.clear();
    z_buffer.shrink_to_fit();
}