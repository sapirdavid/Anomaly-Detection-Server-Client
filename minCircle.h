
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Point get_circle_center(Point p1, Point p2, Point p3);

float get_distance(Point p1, Point p2);

float get_circle_radius(Point a, Point center);

Circle circle_from_3_points(Point p1, Point p2, Point p3);

Circle circle_from_2_points(Point p1, Point p2);

Circle get_trivial_circle(vector<Point *> points, int N);

bool is_point_inside(Circle c, Point p, float num);

Circle help_find_min_circle(Point **points, vector<Point *> boundary_points, size_t size);

Circle findMinCircle(Point **points, size_t size);

#endif /* MINCIRCLE_H_ */
