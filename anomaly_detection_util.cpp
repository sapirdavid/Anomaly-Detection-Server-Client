/*
 * anomaly_detection_util.cpp
 * Author: Sapir David.
 * ID: 318574712.
 */
#include "anomaly_detection_util.h"
#include <cmath>

//calculate the average of array's members.
float avg(float* x, int size) {
    float sum = 0, average;
    for (int i = 0; i < size; i++) {
        sum = sum + x[i];
    }
    average = sum / size;
    return average;
}
// returns the variance of X and Y.
float var(float* x, int size){
    float sumSquares = 0, expectedValue, variance;
    for (int i = 0; i < size; i++) {
        sumSquares = sumSquares + (x[i] * x[i]);
    }
    expectedValue = avg(x, size);
    variance = (sumSquares / size) - (expectedValue * expectedValue);
    return variance;
}
// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float avgX, avgY, covariance, sum = 0;
    avgX = avg(x, size);
    avgY = avg(y, size);
    for (int i = 0; i < size; i++) {
        sum = sum + ((x[i] - avgX) * (y[i] - avgY));
    }
    covariance = sum / size;
    return covariance;
}
// returns the Pearson correlation coefficient of X and Y.
float pearson(float* x, float* y, int size){
    float covariance, standardDeviationX, standardDeviationY;
    covariance = cov(x, y, size);
    standardDeviationX = sqrt(var(x, size));
    standardDeviationY = sqrt(var(y, size));
    return (covariance / (standardDeviationX * standardDeviationY));
}
// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    float a, b;
    float x[size], y[size];
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    a = (cov(x, y, size)) / (var(x, size));
    b = avg(y, size) - (a * avg(x, size));
    Line line(a,b);
    return line;
}
// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
    float xValue, yValue, a, b, dev;
    Line line = linear_reg(points, size);
    a = line.a;
    b = line.b;
    xValue = p.x;
    yValue = a * xValue + b;
    dev = yValue - p.y;
    if ((dev) < 0) {
        dev = -1 * dev;
    }
    return dev;
}
// returns the deviation between point p and the line
float dev(Point p,Line l){
    float xValue, yValue, a, b, dev;
    a = l.a;
    b = l.b;
    xValue = p.x;
    yValue = a * xValue + b;
    dev = yValue - p.y;
    if ((dev) < 0) {
        dev = -1 * dev;
    }
    return dev;
}




