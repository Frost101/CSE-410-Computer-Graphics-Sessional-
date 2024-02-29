#ifndef Z_BUFFER_H
#define Z_BUFFER_H

#include<bits/stdc++.h>
#include "helper.h"
#include "Point.h"
#include "bitmap_image.hpp"

using namespace std;

class ZBuffer{
    public:
    int screen_width, screen_height;
    double right_limit, left_limit;
    double top_limit, bottom_limit;
    double dx;
    double dy;

    double top_Y;
    double bottom_Y;

    double left_X;
    double right_X;

    double Z_MAX, Z_MIN;

    vector<vector<double>> z_buffer;


    ZBuffer(int width, int height){
        screen_width = width;
        screen_height = height;

        right_limit = 1.0, left_limit = -1.0;
        top_limit = 1.0, bottom_limit = -1.0;
        dx = (right_limit - left_limit) / screen_width;
        dy = (top_limit - bottom_limit) / screen_height;

        top_Y = top_limit - (dy / 2.0);
        bottom_Y = bottom_limit + (dy / 2.0);

        left_X = left_limit + (dx / 2.0);
        right_X = right_limit - (dx / 2.0);

        Z_MAX = 1.0, Z_MIN = -1.0;

        init();
    }

    void init(){
        z_buffer.resize(screen_height);
        for(int i=0; i<screen_height; i++){
            z_buffer[i].resize(screen_width);
            for(int j=0; j<screen_width; j++){
                z_buffer[i][j] = Z_MAX;
            }
        }
    }


    void output_z_buffer(){
        ofstream fout("z_buffer.txt");
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
    }


    void free_memory(){
        for(int i=0; i<screen_height; i++){
        z_buffer[i].clear();
        }
        z_buffer.clear();
        z_buffer.shrink_to_fit();
    }


    void applyProcedure(){
        ifstream fin("stage3.txt");

        bitmap_image image(screen_width, screen_height);
        image.set_all_channels(0, 0, 0);

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

            if(top_scanline == bottom_scanline){
                continue;
            }

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


                //* If minX and maxX are equal, then continue
                if(x_vals[0] == x_vals[1]){
                    continue;
                }

                int left_intersecting_column = (int) round((x_vals[0] - left_X) / dx);
                int right_intersecting_column = (int) round((x_vals[1] - left_X) / dx);

                for(int col_no = left_intersecting_column; col_no <= right_intersecting_column; col_no++){
                    double x_val = left_X + (col_no * dx);

                    //* Calculate z values
                    // double z_val = z_vals[0] + (z_vals[1] - z_vals[0]) * (x_val - x_vals[0]) / (x_vals[1] - x_vals[0]);
                    double z_val = z_vals[1] - (z_vals[1] - z_vals[0]) * (x_vals[1] - x_val) / (x_vals[1] - x_vals[0]);

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

        output_z_buffer();
        free_memory();
    }
};



#endif