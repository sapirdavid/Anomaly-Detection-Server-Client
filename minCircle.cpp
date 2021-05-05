/*
 * minCircle.cpp
 * Author: Sapir David.
 * ID: 318574712.
 */

#include "minCircle.h"
#include <math.h>
#include <vector>

//find circle center given 3 points
Point get_circle_center(Point p1, Point p2, Point p3) {
    //get x,y values of p1
    float p1_x = p1.x;
    float p1_y = p1.y;
    //calculate the pow of x,y values of p1
    float x1_pow = pow(p1_x, 2);
    float y1_pow = pow(p1_y, 2);
    //add x1,y1 pow of each point (x1^2 + y1^2)
    float pow1_add = x1_pow + y1_pow;

    //get x,y values of p2
    float p2_x = p2.x;
    float p2_y = p2.y;
    //calculate the pow of x,y values of p2
    float x2_pow = pow(p2_x, 2);
    float y2_pow = pow(p2_y, 2);
    //add x2,y2 pow of each point (x2^2 + y2^2)
    float pow2_add = x2_pow + y2_pow;

    //get x,y values of p3
    float p3_x = p3.x;
    float p3_y = p3.y;
    //calculate the pow of x,y values of p3
    float x3_pow = pow(p3_x, 2);
    float y3_pow = pow(p3_y, 2);
    //add x3,y3 pow of each point (x3^2 + y3^2)
    float pow3_add = x3_pow + y3_pow;

    /*
     calculate the center of the circle using the general equation: a(x^2+y^2)+bx+cy+d=0
     insert the three points into the equation and receive one equation for each point
     the following part will be solving each equation to get the center point
     */

    //calculate a value by inserting p1, p2, p3 into the general equation above
    float a = p1_x * (p2_y - p3_y) - p1_y * (p2_x - p3_x) + p2_x * p3_y - p3_x * p2_y;

    //calculate each part of b value by inserting p1, p2, p3 into the general equation above
    float b1 = pow1_add * (p3_y - p2_y);
    float b2 = pow2_add * (p1_y - p3_y);
    float b3 = pow3_add * (p2_y - p1_y);
    float total_b = b1 + b2 + b3;

    //calculate each part of c value by inserting p1, p2, p3 into the general equation above
    float c1 = pow1_add * (p2_x - p3_x);
    float c2 = pow2_add * (p3_x - p1_x);
    float c3 = pow3_add * (p1_x - p2_x);
    float total_c = c1 + c2 + c3;

    float dev_value = -1 / (2 * a);
    float center_x = total_b * dev_value;
    float center_y = total_c * dev_value;
    Point center(center_x, center_y);
    return center;
}

//get distance between two points
float get_distance(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//get the radius of the circle from center and point
float get_circle_radius(Point a, Point center) {
    return get_distance(a, center);
}

//get circle from three points
Circle circle_from_3_points(Point p1, Point p2, Point p3) {
    Point center = get_circle_center(p1, p2, p3);
    float radius = get_circle_radius(p1, center);
    return Circle(center, radius);
}

//get circle from two points
Circle circle_from_2_points(Point p1, Point p2) {
    float center_x = (p1.x + p2.x) / 2;
    float center_y = (p1.y + p2.y) / 2;
    Point center = Point(center_x, center_y);
    float radius = get_distance(p1, p2) / 2;
    return Circle(center, radius);
}

//get trivial circle - for the cases of 1 or 2 or 3 points
Circle get_trivial_circle(vector<Point *> points, int N) {
    if (N == 1) {
        return Circle(*(points.at(0)), 0);
    }
    if (N == 2) {
        return circle_from_2_points(*(points.at(0)), *(points.at(1)));
    }
    if (N == 3) {
        return circle_from_3_points(*(points.at(0)), *(points.at(1)), *(points.at(2)));
    }
}

//check if point is inside the circle
bool is_point_inside(Circle c, Point p, float num) {
    Point center = Point(c.center.x, c.center.y);
    if (get_distance(center, p) <= (c.radius * num)) {
        return true;
    }
    return false;
}

//help function for find the min circle
Circle help_find_min_circle(Point **points, vector<Point *> boundary_points, size_t size) {
    //base condition
    if (size == 0 || boundary_points.size() ==  3) {
        return get_trivial_circle(boundary_points, boundary_points.size());
    }
    //choose a the last point at each interaction
    Point *p = points[size - 1];
    //send the function recursively to find the min circle - we calculate the min circle for all points except  p
    Circle c = help_find_min_circle(points, boundary_points, size - 1);
    //if the circle we have calculated above contains p - than we've finished.
    if (is_point_inside(c, *p, 1)) {
        return c;
    }
    /*if p is not inside the circle, it's on the circle's boundary (of the circle we have calculated).
     * so we save all points on the boundary into a vector*/
    boundary_points.push_back(p);
    //calculate the min circle for the point p
    return help_find_min_circle(points, boundary_points, size - 1);
}

//find the min circle from all points
Circle findMinCircle(Point **points, size_t size) {
    if (size == 1 || size == 2) {
        vector<Point *> boundary_points;
        for (int i = 0; i < size ; i++) {
            boundary_points.push_back(points[i]);
        }
        return get_trivial_circle(boundary_points, size);
    }
    return help_find_min_circle(points, {}, size);
}
